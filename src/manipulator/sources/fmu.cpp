#ifdef _WIN32
#    pragma warning(disable : 4244)
#endif

#ifdef __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "fmu-uuid.h"

#include <cppfmu_cs.hpp>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <xtensor-blas/xlinalg.hpp>
#include <xtensor/xfixed.hpp>
#include <xtensor/xvectorize.hpp>

#define G 9.81
#define PI 3.141592653589793

constexpr int real_value_size = 14;

// CONSTANT PARAMETER INDEXES
constexpr int L1_IDX = 0;
constexpr int L2_IDX = 1;
constexpr int M1_IDX = 2;
constexpr int M2_IDX = 3;
constexpr int Q1_IDX = 4;
constexpr int Q2_IDX = 5;
constexpr int REF1_IDX = 6;
constexpr int REF2_IDX = 7;
constexpr int Kp1_IDX = 8;
constexpr int Kd1_IDX = 9;
constexpr int Ki1_IDX = 10;
constexpr int Kp2_IDX = 11;
constexpr int Kd2_IDX = 12;
constexpr int Ki2_IDX = 13;

using namespace cppfmu;

class TwoDoFManipulator : public cppfmu::SlaveInstance
{
private:
    bool initialized_ {};
    FMIReal l1 {}, l2 {}, m1 {}, m2 {}, ref1 {}, ref2 {}, Kp1, Kd1, Ki1, Kp2, Kd2, Ki2, er1{}, er2{};
    FMIReal values[real_value_size];

    typedef xt::xtensor_fixed<FMIReal, xt::xshape<2, 1>> M21;

    xt::xtensor_fixed<FMIReal, xt::xshape<2, 2>> B {};
    M21 C {};
    M21 dq {};
    M21 ddq {};
    M21 g {};
    M21 q {};

    M21 pid {};

    FMIReal wrap_radians(FMIReal x)
    {
        return x - 2 * PI * floor((x + PI) / (2 * PI));
    }

    template<typename T>
    T xt_wrap_radians(T x)
    {
        return x - 2 * PI * xt::floor((x + PI) / (2 * PI));
    }

    void compute_b()
    {
        B = {
            {(m1 + m2) * pow(l1, 2) + m2 * pow(l2, 2) + 2 * m2 * l1 * l2 * cos(q(1,0)),   m2 * pow(l2, 2) - m2 * l1 * l2 * cos(q(1,0))},
            {m2 * pow(l2, 2) + m2 * l1 * l2 * cos(q(1,0)),                                m2 * pow(l2, 2)}
        };
    }

    void compute_c()
    {
        C = {{
            -m2 * l1 * l2 * sin(q(1,0)) * (2 * dq(0,0) * dq(1,0) + pow(dq(1,0), 2)),
            -m2 * l1 * l2 * sin(q(1,0)) * dq(0,0) * dq(1,0)
        }};
    }

    void compute_g()
    {
        g = {{
            -(m1 + m2) * l1 * G * sin(q(0,0)) - m2 * l2 * G * sin(q(0,0) + q(1,0)),
            -m2 * l2 * G * sin(q(0,0) + q(1,0))
        }};
    }

    void compute_pid()
    {
        auto e1 = wrap_radians(ref1 - q(0, 0));
        auto e2 = wrap_radians(ref2 - q(1, 0));
        er1 += e1;
        er2 += e2;

        pid = {{
            Kp1 * e1 - Kd1 * dq(0, 0) + Ki1 * er1,
            Kp2 * e2 - Kd2 * dq(1, 0) + Ki2 * er2
        }};

    }

public:
    TwoDoFManipulator() { TwoDoFManipulator::Reset(); }

    void Reset() override
    {
    }

    void SetReal(
        const cppfmu::FMIValueReference vr[],
        std::size_t nvr,
        const cppfmu::FMIReal value[]) override
    {
        for (size_t i = 0; i < nvr; i++) {
            values[vr[i]] = value[i];
        }
        if (!initialized_) {
            l1  = values[L1_IDX];
            l2  = values[L2_IDX];
            m1  = values[M1_IDX];
            m2  = values[M2_IDX];
            Kp1 = values[Kp1_IDX];
            Kp2 = values[Kp1_IDX];
            Kd1 = values[Kd1_IDX];
            Kd2 = values[Kd1_IDX];
            Ki1 = values[Ki1_IDX];
            Ki2 = values[Ki1_IDX];
            initialized_ = true;
        }
        ref1 = values[REF1_IDX];
        ref2 = values[REF2_IDX];
    }

    void GetReal(
        const cppfmu::FMIValueReference vr[],
        std::size_t nvr,
        cppfmu::FMIReal value[]) const override
    {
        value[Q1_IDX] = q(0,0);
        value[Q2_IDX] = q(1,0);
    }

    bool DoStep(cppfmu::FMIReal t,
        cppfmu::FMIReal h,
        cppfmu::FMIBoolean newStep,
        cppfmu::FMIReal& endOfStep) override
    {
        compute_b();
        compute_c();
        compute_g();
        compute_pid();

        ddq = xt::linalg::dot(xt::linalg::inv(B), (-C - g)) + pid;
        dq = dq + ddq * h;
        q = xt_wrap_radians<M21>(q + dq * h);

        return true;
    }
};

cppfmu::UniquePtr<cppfmu::SlaveInstance> CppfmuInstantiateSlave(
    cppfmu::FMIString /*instanceName*/, cppfmu::FMIString fmuGUID,
    cppfmu::FMIString /*fmuResourceLocation*/, cppfmu::FMIString /*mimeType*/,
    cppfmu::FMIReal /*timeout*/, cppfmu::FMIBoolean /*visible*/,
    cppfmu::FMIBoolean /*interactive*/, cppfmu::Memory memory,
    cppfmu::Logger /*logger*/)
{
    if (std::strcmp(fmuGUID, FMU_UUID) != 0) {
        throw std::runtime_error("FMU GUID mismatch");
    }
    return cppfmu::AllocateUnique<TwoDoFManipulator>(memory);
}

#ifdef __GNUC__
#    pragma GCC diagnostic pop
#endif
