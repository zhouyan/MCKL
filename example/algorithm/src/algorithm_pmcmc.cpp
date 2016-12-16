//============================================================================
// MCKL/example/algorithm/src/algorithm_pmcmc.cpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2017, Yan Zhou
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

#include "algorithm_pmcmc.hpp"

int main()
{
    constexpr std::size_t K = 80;
    constexpr std::size_t M = 10;
    constexpr std::size_t N = 1000;

    mckl::PMCMCMutation<AlgorithmPMCMCState, AlgorithmPMCMC> mutation(
        AlgorithmPMCMCPrior(), N);
    mutation.update(AlgorithmPMCMCUpdate<0>());
    mutation.update(AlgorithmPMCMCUpdate<1>());
    mutation.pf().selection(AlgorithmPMCMCSelection());
    mutation.pf().resample(mckl::Stratified);
    mutation.pf().resample_threshold(0.5);

    mckl::MCMCSampler<AlgorithmPMCMCState> sampler;
    sampler.mutation(std::move(mutation));
    auto &monitor = sampler
                        .monitor(mckl::MCMCMonitor<AlgorithmPMCMCState>(
                            2, AlgorithmPMCMCEstimate()))
                        .second;

    std::get<0>(sampler.state()) = 0.10;
    std::get<1>(sampler.state()) = 10.0;
    std::cout << std::string(K, '=') << std::endl;
    for (std::size_t i = 0; i != K; ++i) {
        sampler.iterate(M);
        std::cout << '-' << std::flush;
    }
    std::cout << std::endl;

    double mean[2];
    monitor.result(mean, 500, 7);
    std::cout << "theta: " << mean[0] << std::endl;
    std::cout << "simga: " << mean[1] << std::endl;

    std::ofstream save("algorithm_pmcmc.save");
    save << sampler << std::endl;
    save.close();

    return 0;
}
