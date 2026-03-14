#include <cassert>
#include <cmath>
#include <functional>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include <bayestest.hpp>

using bayestest::BinaryTest;
using bayestest::CountTest;
using bayestest::detail::prob_b_beats_a;
using bayestest::detail::prob_c_beats_ab;
using bayestest::detail::prob_d_beats_abc;
using bayestest::detail::prob_1_beats_2;
using bayestest::detail::prob_1_beats_23;

void assert_approx(double act, double exp) {
    assert(std::abs(act - exp) < 0.0000000001);
}

template<typename T>
void assert_exception(const std::function<void(void)>& code, const std::string& expected) {
    std::string message = "";
    try {
        code();
    } catch (const T& e) {
        message = std::string{e.what()};
    }
    assert(message == expected);
}

void test_binary_no_variants() {
    BinaryTest test;
    assert(test.probabilities().empty());
}

void test_binary_one_variant() {
    BinaryTest test;
    test.add(2, 1);
    std::vector<double> expected = {1.0};
    assert(test.probabilities() == expected);
}

void test_binary_two_variants() {
    BinaryTest test;
    test.add(200, 100);
    test.add(400, 250);
    std::vector<double> probabilities = test.probabilities();
    assert(probabilities.size() == 2);
    assert_approx(probabilities.at(0), 0.001756431311879969);
    assert_approx(probabilities.at(1), 0.99824356868812);
}

void test_binary_three_variants() {
    BinaryTest test;
    test.add(61, 15);
    test.add(54, 13);
    test.add(72, 19);
    std::vector<double> probabilities = test.probabilities();
    assert(probabilities.size() == 3);
    assert_approx(probabilities.at(0), 0.29632930651329037);
    assert_approx(probabilities.at(1), 0.277257277195332);
    assert_approx(probabilities.at(2), 0.42641341629137763);
}

void test_binary_four_variants() {
    BinaryTest test;
    test.add(55, 50);
    test.add(30, 30);
    test.add(10, 10);
    test.add(50, 45);
    std::vector<double> probabilities = test.probabilities();
    assert(probabilities.size() == 4);
    assert_approx(probabilities.at(0), 0.02692341639320739);
    assert_approx(probabilities.at(1), 0.7040521621641954);
    assert_approx(probabilities.at(2), 0.249824960767943);
    assert_approx(probabilities.at(3), 0.019199460674668434);
}

void test_binary_five_variants() {
    BinaryTest test;
    assert_exception<std::runtime_error>([&]() {
        for (int i = 0; i < 5; i++) {
            test.add(2, 1);
        }
    }, "too many variants");
}

void test_binary_negative_participants() {
    BinaryTest test;
    assert_exception<std::invalid_argument>([&]() {
        test.add(-1, 1);
    }, "participants cannot be negative");
}

void test_binary_high_participants() {
    BinaryTest test;
    assert_exception<std::invalid_argument>([&]() {
        test.add(std::numeric_limits<int>::max(), 1);
    }, "too many participants");
}

void test_binary_negative_conversions() {
    BinaryTest test;
    assert_exception<std::invalid_argument>([&]() {
        test.add(1, -1);
    }, "conversions cannot be negative");
}

void test_binary_high_conversions() {
    BinaryTest test;
    assert_exception<std::invalid_argument>([&]() {
        test.add(1, std::numeric_limits<int>::max());
    }, "too many conversions");
}

void test_binary_more_conversions() {
    BinaryTest test;
    assert_exception<std::invalid_argument>([&]() {
        test.add(1, 2);
    }, "conversions cannot be greater than participants");
}

void test_count_no_variants() {
    CountTest test;
    assert(test.probabilities().empty());
}

void test_count_one_variant() {
    CountTest test;
    test.add(2, 1);
    std::vector<double> expected = {1.0};
    assert(test.probabilities() == expected);
}

void test_count_two_variants() {
    CountTest test;
    test.add(55, 50);
    test.add(30, 30);
    std::vector<double> probabilities = test.probabilities();
    assert(probabilities.size() == 2);
    assert_approx(probabilities.at(0), 0.6710529663661625);
    assert_approx(probabilities.at(1), 0.3289470336338596);
}

void test_count_three_variants() {
    CountTest test;
    test.add(55, 50);
    test.add(30, 30);
    test.add(10, 10);
    std::vector<double> probabilities = test.probabilities();
    assert(probabilities.size() == 3);
    assert_approx(probabilities.at(0), 0.4633365654508068);
    assert_approx(probabilities.at(1), 0.2306153779716283);
    assert_approx(probabilities.at(2), 0.3060480565775272);
}

void test_count_four_variants() {
    CountTest test;
    assert_exception<std::runtime_error>([&]() {
        for (int i = 0; i < 5; i++) {
            test.add(2, 1);
        }
    }, "too many variants");
}

void test_count_exposure_relative() {
    CountTest test;
    test.add(55, 50000);
    test.add(30, 30000);
    std::vector<double> probabilities = test.probabilities();
    assert(probabilities.size() == 2);
    assert_approx(probabilities.at(0), 0.6710529663661625);
    assert_approx(probabilities.at(1), 0.3289470336338596);
}

void test_count_negative_events() {
    CountTest test;
    assert_exception<std::invalid_argument>([&]() {
        test.add(-1, 1);
    }, "events cannot be negative");
}

void test_count_high_events() {
    CountTest test;
    assert_exception<std::invalid_argument>([&]() {
        test.add(std::numeric_limits<int>::max(), 1);
    }, "too many events");
}

void test_count_negative_exposure() {
    CountTest test;
    assert_exception<std::invalid_argument>([&]() {
        test.add(1, -1);
    }, "exposure cannot be negative");
}

void test_count_high_exposure() {
    CountTest test;
    assert_exception<std::invalid_argument>([&]() {
        test.add(1, std::numeric_limits<int>::max());
    }, "too high exposure");
}

void test_prob_b_beats_a() {
    assert_approx(prob_b_beats_a(1, 2, 3, 4), 0.6428571428571429);
    assert_approx(prob_b_beats_a(55, 50, 30, 30), 0.38386463776317903);
    assert_approx(prob_b_beats_a(50, 50, 35, 30), 0.6867997222295887);
}

void test_prob_c_beats_ab() {
    assert_approx(prob_c_beats_ab(1, 2, 3, 4, 5, 6), 0.4128959276018096);
    assert_approx(prob_c_beats_ab(1, 2, 3, 4, 5, 100), 0.0004147782900546515);
    assert_approx(prob_c_beats_ab(55, 50, 30, 30, 10, 10), 0.35421204276865736);
    assert_approx(prob_c_beats_ab(50, 50, 35, 30, 13, 18), 0.09139305107602524);
}

void test_prob_d_beats_abc() {
    assert_approx(prob_d_beats_abc(1, 2, 3, 4, 5, 6, 7, 8), 0.2853316096371013);
    assert_approx(prob_d_beats_abc(55, 50, 30, 30, 10, 10, 25, 30), 0.08421499131901738);
}

void test_prob_1_beats_2() {
    assert_approx(prob_1_beats_2(1, 2, 3, 4), 0.29629629629629595);
    assert_approx(prob_1_beats_2(55, 50, 30, 30), 0.6710529663661625);
    assert_approx(prob_1_beats_2(50, 50, 35, 30), 0.24796547380927997);
}

void test_prob_1_beats_23() {
    assert_approx(prob_1_beats_23(1, 2, 3, 4, 5, 6), 0.16901765046296247);
    assert_approx(prob_1_beats_23(1, 2, 3, 4, 5, 100), 0.2962330601144884);
    assert_approx(prob_1_beats_23(55, 50, 30, 30, 10, 10), 0.4633365654508068);
    assert_approx(prob_1_beats_23(50, 50, 35, 30, 13, 18), 0.23397153850438435);
}

int main() {
    test_binary_no_variants();
    test_binary_one_variant();
    test_binary_two_variants();
    test_binary_three_variants();
    test_binary_four_variants();
    test_binary_five_variants();
    test_binary_negative_participants();
    test_binary_high_participants();
    test_binary_negative_conversions();
    test_binary_high_conversions();
    test_binary_more_conversions();

    test_count_no_variants();
    test_count_one_variant();
    test_count_two_variants();
    test_count_three_variants();
    test_count_four_variants();
    test_count_exposure_relative();
    test_count_negative_events();
    test_count_high_events();
    test_count_negative_exposure();
    test_count_high_exposure();

    test_prob_b_beats_a();
    test_prob_c_beats_ab();
    test_prob_d_beats_abc();

    test_prob_1_beats_2();
    test_prob_1_beats_23();

    return 0;
}
