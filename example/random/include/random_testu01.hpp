//============================================================================
// MCKL/example/random/include/random_testu01.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2016, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#ifndef MCKL_EXAMPLE_RANDOM_TESTU01_HPP
#define MCKL_EXAMPLE_RANDOM_TESTU01_HPP

#define MCKL_U01_USE_FIXED_POINT 0

#define MCKL_U01_USE_64BITS_DOUBLE 0

#include <mckl/random/testu01.hpp>
#include "random_common.hpp"

template <typename RNGType, typename U01Type, typename Battery>
inline void random_testu01(
    const std::string &name, Battery &&battery, mckl::Vector<int> &rep)
{
    mckl::TestU01 &testu01 = mckl::TestU01::instance();
    testu01.reset<RNGType, U01Type>(name);
    rep.size() == 0 ?
        testu01(std::forward<Battery>(battery), 1) :
        testu01(std::forward<Battery>(battery), rep.begin(), rep.end(), 2);
}

template <typename RNGType, typename Battery>
inline void random_testu01(Battery &&battery, int argc, char **argv)
{
    std::string basename(*argv);
    --argc;
    ++argv;

    bool STD = false;
    bool U01 = false;
    bool U01CC = false;
    bool U01CO = false;
    bool U01OC = false;
    bool U01OO = false;
    bool verbose = false;
    bool redirect = true;
    bool all = true;
    mckl::Vector<int> rep;
    while (argc > 0) {
        std::string arg(*argv);
        if (arg.find("STD") != std::string::npos) {
            STD = true;
            all = false;
        } else if (arg.find("U01CC") != std::string::npos) {
            U01CC = true;
            all = false;
        } else if (arg.find("U01CO") != std::string::npos) {
            U01CO = true;
            all = false;
        } else if (arg.find("U01OC") != std::string::npos) {
            U01OC = true;
            all = false;
        } else if (arg.find("U01OO") != std::string::npos) {
            U01OO = true;
            all = false;
        } else if (arg.find("U01") != std::string::npos) {
            U01 = true;
            all = false;
        } else if (arg.find("verbose") != std::string::npos) {
            verbose = true;
        } else if (arg.find("stdout") != std::string::npos) {
            redirect = false;
        } else {
            rep.push_back(std::atoi(arg.c_str()));
        }
        --argc;
        ++argv;
    }

    if (all) {
        STD = true;
        U01 = true;
        U01CC = true;
        U01CO = true;
        U01OC = true;
        U01OO = true;
    }

    const unsigned seed =
        1U + (rep.size() == 0 ? 0U : std::thread::hardware_concurrency() * 6U);

    std::string seedfile(basename + ".seed");
    std::ifstream seedin(seedfile);
    if (seedin)
        seedin >> mckl::Seed<RNGType>::instance();
    else
        mckl::Seed<RNGType>::instance().set(seed);
    seedin.close();

    if (redirect) {
        std::string resultfile(basename);
        if (!all) {
            if (STD)
                resultfile += "_std";
            if (U01)
                resultfile += "_u01";
            if (U01CC)
                resultfile += "_u01cc";
            if (U01CO)
                resultfile += "_u01co";
            if (U01OC)
                resultfile += "_u01oc";
            if (U01OO)
                resultfile += "_u01oo";
        }
        resultfile += ".txt";
        std::freopen(resultfile.c_str(), "w", stdout);
    }

    if (!verbose)
        ::swrite_Basic = FALSE;

    if (STD) {
        random_testu01<RNGType, std::uniform_real_distribution<double>>(
            "STD", std::forward<Battery>(battery), rep);
    }
    if (U01) {
        random_testu01<RNGType, mckl::U01Distribution<double>>(
            "U01", std::forward<Battery>(battery), rep);
    }
    if (U01CC) {
        random_testu01<RNGType, mckl::U01CCDistribution<double>>(
            "U01CC", std::forward<Battery>(battery), rep);
    }
    if (U01CO) {
        random_testu01<RNGType, mckl::U01CODistribution<double>>(
            "U01CO", std::forward<Battery>(battery), rep);
    }
    if (U01OC) {
        random_testu01<RNGType, mckl::U01OCDistribution<double>>(
            "U01OC", std::forward<Battery>(battery), rep);
    }
    if (U01OO) {
        random_testu01<RNGType, mckl::U01OODistribution<double>>(
            "U01OO", std::forward<Battery>(battery), rep);
    }

    if (redirect)
        std::fclose(stdout);

    std::ofstream seedout(seedfile);
    if (seedout)
        seedout << mckl::Seed<RNGType>::instance() << std::endl;
    seedout.close();
}

#endif // MCKL_EXAMPLE_RANDOM_TESTU01_HPP
