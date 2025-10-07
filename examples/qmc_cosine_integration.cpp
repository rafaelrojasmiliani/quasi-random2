#include "quasirand.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>

namespace
{
    template<std::size_t Dim>
    double integrate_cosine_product(std::size_t sample_count)
    {
        quasirand::QuasiRandom<Dim> generator;

        const double upper_bound = std::acos(-1.0) / 2.0; // pi / 2
        double estimate = 0.0;

        for (std::size_t i = 0; i < sample_count; ++i)
        {
            const auto& unit_point = generator();
            double value = 1.0;

            for (double coordinate : unit_point)
            {
                const double x = coordinate * upper_bound;
                value *= std::cos(x);
            }

            estimate += value;
        }

        const double hypercube_volume = std::pow(upper_bound, Dim);
        return hypercube_volume * estimate / static_cast<double>(sample_count);
    }
}

int main()
{
    constexpr std::size_t dim = 5;
    constexpr std::size_t sample_count = 10'000; // 10,000 samples

    const double upper_bound = std::acos(-1.0) / 2.0; // pi / 2
    const double qmc_estimate = integrate_cosine_product<dim>(sample_count);
    const double exact_value = std::pow(std::sin(upper_bound), dim);
    const double absolute_error = std::abs(qmc_estimate - exact_value);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Integrating f(x) = ";
    std::cout << "\\u220F_{i=0}^{" << dim - 1 << "} cos(x_i) on [0, pi/2]^" << dim << '\n';
    std::cout << "Quasi Monte Carlo estimate: " << qmc_estimate << '\n';
    std::cout << "Exact value: " << exact_value << '\n';
    std::cout << "Absolute error: " << absolute_error << '\n';

    return 0;
}
