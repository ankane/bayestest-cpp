/*
 * BayesTest C++ v0.1.2
 * https://github.com/ankane/bayestest-cpp
 * MIT License
 */

#pragma once

#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

namespace bayestest {

namespace detail {

inline double logbeta(double a, double b) {
    return std::lgamma(a) + std::lgamma(b) - std::lgamma(a + b);
}

inline double prob_b_beats_a(int alpha_a, int beta_a, int alpha_b, int beta_b) {
    double total = 0.0;
    double logbeta_aa_ba = logbeta(alpha_a, beta_a);
    double beta_ba = beta_b + beta_a;

    for (int i = 0; i < alpha_b; i++) {
        total += std::exp(logbeta(alpha_a + i, beta_ba) - std::log(beta_b + i) - logbeta(1 + i, beta_b) - logbeta_aa_ba);
    }

  return total;
}

inline double prob_c_beats_ab(int alpha_a, int beta_a, int alpha_b, int beta_b, int alpha_c, int beta_c) {
    double total = 0.0;

    double logbeta_ac_bc = logbeta(alpha_c, beta_c);

    std::vector<double> log_bb_j_logbeta_j_bb;
    log_bb_j_logbeta_j_bb.reserve(static_cast<size_t>(alpha_b));

    for (int j = 0; j < alpha_b; j++) {
        log_bb_j_logbeta_j_bb.push_back(std::log(beta_b + j) + logbeta(1 + j, beta_b));
    }

    double abc = beta_a + beta_b + beta_c;
    std::vector<double> logbeta_ac_i_j;
    logbeta_ac_i_j.reserve(static_cast<size_t>(alpha_a) + static_cast<size_t>(alpha_b));

    for (int i = 0; i < alpha_a + alpha_b; i++) {
        logbeta_ac_i_j.push_back(logbeta(alpha_c + i, abc));
    }

    for (int i = 0; i < alpha_a; i++) {
        double sum_i = -std::log(beta_a + i) - logbeta(1 + i, beta_a) - logbeta_ac_bc;

        for (int j = 0; j < alpha_b; j++) {
            total += std::exp(sum_i + logbeta_ac_i_j.at(static_cast<size_t>(i) + static_cast<size_t>(j)) - log_bb_j_logbeta_j_bb.at(static_cast<size_t>(j)));
        }
    }

    return 1
        - prob_b_beats_a(alpha_c, beta_c, alpha_a, beta_a)
        - prob_b_beats_a(alpha_c, beta_c, alpha_b, beta_b)
        + total;
}

inline double prob_d_beats_abc(int alpha_a, int beta_a, int alpha_b, int beta_b, int alpha_c, int beta_c, int alpha_d, int beta_d) {
    double total = 0.0;

    double logbeta_ad_bd = logbeta(alpha_d, beta_d);

    std::vector<double> log_bb_j_logbeta_j_bb;
    log_bb_j_logbeta_j_bb.reserve(static_cast<size_t>(alpha_b));

    for (int j = 0; j < alpha_b; j++) {
        log_bb_j_logbeta_j_bb.push_back(std::log(beta_b + j) + logbeta(1 + j, beta_b));
    }

    std::vector<double> log_bc_k_logbeta_k_bc;
    log_bc_k_logbeta_k_bc.reserve(static_cast<size_t>(alpha_c));

    for (int k = 0; k < alpha_c; k++) {
        log_bc_k_logbeta_k_bc.push_back(std::log(beta_c + k) + logbeta(1 + k, beta_c));
    }

    double abcd = beta_a + beta_b + beta_c + beta_d;
    std::vector<double> logbeta_bd_i_j_k;
    logbeta_bd_i_j_k.reserve(static_cast<size_t>(alpha_a) + static_cast<size_t>(alpha_b) + static_cast<size_t>(alpha_c));

    for (int i = 0; i < alpha_a + alpha_b + alpha_c; i++) {
        logbeta_bd_i_j_k.push_back(logbeta(alpha_d + i, abcd));
    }

    for (int i = 0; i < alpha_a; i++) {
        double sum_i = -std::log(beta_a + i) - logbeta(1 + i, beta_a) - logbeta_ad_bd;

        for (int j = 0; j < alpha_b; j++) {
            double sum_j = sum_i - log_bb_j_logbeta_j_bb.at(static_cast<size_t>(j));

            for (int k = 0; k < alpha_c; k++) {
                total += std::exp(sum_j + logbeta_bd_i_j_k.at(static_cast<size_t>(i) + static_cast<size_t>(j) + static_cast<size_t>(k)) - log_bc_k_logbeta_k_bc.at(static_cast<size_t>(k)));
            }
        }
    }

    return 1
        - prob_b_beats_a(alpha_a, beta_a, alpha_d, beta_d)
        - prob_b_beats_a(alpha_b, beta_b, alpha_d, beta_d)
        - prob_b_beats_a(alpha_c, beta_c, alpha_d, beta_d)
        + prob_c_beats_ab(alpha_a, beta_a, alpha_b, beta_b, alpha_d, beta_d)
        + prob_c_beats_ab(alpha_a, beta_a, alpha_c, beta_c, alpha_d, beta_d)
        + prob_c_beats_ab(alpha_b, beta_b, alpha_c, beta_c, alpha_d, beta_d)
        - total;
}

inline double prob_1_beats_2(int alpha_1, int beta_1, int alpha_2, int beta_2) {
    double total = 0.0;
    double log_b1 = std::log(beta_1);
    double a2_log_b2 = alpha_2 * std::log(beta_2);
    double log_b1_b2 = std::log(beta_1 + beta_2);

    for (int k = 0; k < alpha_1; k++) {
        total += std::exp(k * log_b1 +
            a2_log_b2 -
            (k + alpha_2) * log_b1_b2 -
            std::log(k + alpha_2) -
            logbeta(k + 1, alpha_2));
    }

    return total;
}

inline double prob_1_beats_23(int alpha_1, int beta_1, int alpha_2, int beta_2, int alpha_3, int beta_3) {
    double total = 0.0;

    double log_b1_b2_b3 = std::log(beta_1 + beta_2 + beta_3);
    double a1_log_b1 = alpha_1 * std::log(beta_1);
    double log_b2 = std::log(beta_2);
    double log_b3 = std::log(beta_3);
    double loggamma_a1 = std::lgamma(alpha_1);

    for (int k = 0; k < alpha_2; k++) {
        double sum_k = a1_log_b1 + k * log_b2 - std::lgamma(k + 1);

        for (int l = 0; l < alpha_3; l++) {
            total += std::exp(sum_k + l * log_b3
                - (k + l + alpha_1) * log_b1_b2_b3
                + std::lgamma(k + l + alpha_1) - std::lgamma(l + 1) - loggamma_a1);
        }
    }

    return 1
        - prob_1_beats_2(alpha_2, beta_2, alpha_1, beta_1)
        - prob_1_beats_2(alpha_3, beta_3, alpha_1, beta_1)
        + total;
}

} // namespace detail

/// A test for binary outcomes.
class BinaryTest {
  public:
    /// Adds a new variant.
    void add(int participants, int conversions) {
        if (participants < 0) {
            throw std::invalid_argument{"participants cannot be negative"};
        }
        if (participants > std::numeric_limits<int>::max() / static_cast<int>(sizeof(double)) / 4) {
            throw std::invalid_argument{"too many participants"};
        }
        if (conversions < 0) {
            throw std::invalid_argument{"conversions cannot be negative"};
        }
        if (conversions > std::numeric_limits<int>::max() / static_cast<int>(sizeof(double)) / 4) {
            throw std::invalid_argument{"too many conversions"};
        }
        if (conversions > participants) {
            throw std::invalid_argument{"conversions cannot be greater than participants"};
        }
        variants.emplace_back(participants, conversions);
    }

    /// Returns the winning probability of each variant.
    std::vector<double> probabilities() const {
        std::vector<double> probs;
        probs.reserve(variants.size());

        switch (variants.size()) {
            case 0: {
              break;
            }
            case 1: {
                probs.push_back(1);

                break;
            }
            case 2: {
                const Variant& b = variants.at(0);
                const Variant& a = variants.at(1);

                double prob = detail::prob_b_beats_a(
                    1 + a.conversions,
                    1 + a.participants - a.conversions,
                    1 + b.conversions,
                    1 + b.participants - b.conversions
                );
                probs.push_back(prob);
                probs.push_back(1 - prob);

                break;
            }
            case 3: {
                double total = 0.0;
                for (size_t i = 0; i < 2; i++) {
                    const Variant& c = variants.at(i);
                    const Variant& b = variants.at((i + 1) % 3);
                    const Variant& a = variants.at((i + 2) % 3);

                    double prob = detail::prob_c_beats_ab(
                        1 + a.conversions,
                        1 + a.participants - a.conversions,
                        1 + b.conversions,
                        1 + b.participants - b.conversions,
                        1 + c.conversions,
                        1 + c.participants - c.conversions
                    );

                    probs.push_back(prob);
                    total += prob;
                }
                probs.push_back(1 - total);

                break;
            }
            default: {
                double total = 0.0;
                for (size_t i = 0; i < 3; i++) {
                    const Variant& d = variants.at(i);
                    const Variant& c = variants.at((i + 1) % 4);
                    const Variant& b = variants.at((i + 2) % 4);
                    const Variant& a = variants.at((i + 3) % 4);

                    double prob = detail::prob_d_beats_abc(
                        1 + a.conversions,
                        1 + a.participants - a.conversions,
                        1 + b.conversions,
                        1 + b.participants - b.conversions,
                        1 + c.conversions,
                        1 + c.participants - c.conversions,
                        1 + d.conversions,
                        1 + d.participants - d.conversions
                    );

                    probs.push_back(prob);
                    total += prob;
                }
                probs.push_back(1 - total);
            }
        }
        return probs;
  }

  private:
    struct Variant {
        Variant(int participants, int conversions) : participants(participants), conversions(conversions) {}
        int participants;
        int conversions;
    };

    std::vector<Variant> variants;
};

/// A test for count data.
class CountTest {
  public:
    /// Adds a new variant.
    void add(int events, int exposure) {
        if (events < 0) {
            throw std::invalid_argument{"events cannot be negative"};
        }
        if (exposure < 0) {
            throw std::invalid_argument{"exposure cannot be negative"};
        }
        variants.emplace_back(events, exposure);
    }

    /// Returns the winning probability of each variant.
    std::vector<double> probabilities() const {
        std::vector<double> probs;
        probs.reserve(variants.size());

        switch (variants.size()) {
            case 0: {
                break;
            }
            case 1: {
                probs.push_back(1);

                break;
            }
            case 2: {
                const Variant& a = variants.at(0);
                const Variant& b = variants.at(1);

                double prob = detail::prob_1_beats_2(
                    a.events,
                    a.exposure,
                    b.events,
                    b.exposure
                );
                probs.push_back(prob);
                probs.push_back(1 - prob);

                break;
            }
            default: {
                double total = 0.0;
                for (size_t i = 0; i < 2; i++) {
                    const Variant& a = variants.at(i);
                    const Variant& b = variants.at((i + 1) % 3);
                    const Variant& c = variants.at((i + 2) % 3);

                    double prob = detail::prob_1_beats_23(
                      a.events,
                      a.exposure,
                      b.events,
                      b.exposure,
                      c.events,
                      c.exposure
                    );

                    probs.push_back(prob);
                    total += prob;
                }
                probs.push_back(1 - total);
            }
        }
        return probs;
    }

  private:
    struct Variant {
        Variant(int events, int exposure) : events(events), exposure(exposure) {}
        int events;
        int exposure;
    };

    std::vector<Variant> variants;
};

} // namespace bayestest
