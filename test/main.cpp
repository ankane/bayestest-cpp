#include <cassert>
#include "../include/bayestest.hpp"

using bayestest::BinaryTest;
using bayestest::CountTest;
using bayestest::prob_b_beats_a;
using bayestest::prob_c_beats_ab;
using bayestest::prob_d_beats_abc;
using bayestest::prob_1_beats_2;
using bayestest::prob_1_beats_23;

void assert_approx(double act, double exp) {
  assert(abs(act - exp) < 0.0000000001);
}

void test_binary_no_variants() {
  auto test = BinaryTest();
  assert(test.probabilities().empty());
}

void test_binary_one_variant() {
  auto test = BinaryTest();
  test.add(2, 1);
  std::vector<double> expected = {1.0};
  assert(test.probabilities() == expected);
}

void test_binary_two_variants() {
  auto test = BinaryTest();
  test.add(200, 100);
  test.add(400, 250);
  auto probabilities = test.probabilities();
  assert(probabilities.size() == 2);
  assert_approx(probabilities[0], 0.001756431311879969);
  assert_approx(probabilities[1], 0.99824356868812);
}

void test_binary_three_variants() {
  auto test = BinaryTest();
  test.add(61, 15);
  test.add(54, 13);
  test.add(72, 19);
  auto probabilities = test.probabilities();
  assert(probabilities.size() == 3);
  assert_approx(probabilities[0], 0.29632930651329037);
  assert_approx(probabilities[1], 0.277257277195332);
  assert_approx(probabilities[2], 0.42641341629137763);
}

void test_binary_four_variants() {
  auto test = BinaryTest();
  test.add(55, 50);
  test.add(30, 30);
  test.add(10, 10);
  test.add(50, 45);
  auto probabilities = test.probabilities();
  assert(probabilities.size() == 4);
  assert_approx(probabilities[0], 0.02692341639320739);
  assert_approx(probabilities[1], 0.7040521621641954);
  assert_approx(probabilities[2], 0.249824960767943);
  assert_approx(probabilities[3], 0.019199460674668434);
}

void test_count_no_variants() {
  auto test = CountTest();
  assert(test.probabilities().empty());
}

void test_count_one_variant() {
  auto test = CountTest();
  test.add(2, 1);
  std::vector<double> expected = {1.0};
  assert(test.probabilities() == expected);
}

void test_count_two_variants() {
  auto test = CountTest();
  test.add(55, 50);
  test.add(30, 30);
  auto probabilities = test.probabilities();
  assert(probabilities.size() == 2);
  assert_approx(probabilities[0], 0.6710529663661625);
  assert_approx(probabilities[1], 0.3289470336338596);
}

void test_count_three_variants() {
  auto test = CountTest();
  test.add(55, 50);
  test.add(30, 30);
  test.add(10, 10);
  auto probabilities = test.probabilities();
  assert(probabilities.size() == 3);
  assert_approx(probabilities[0], 0.4633365654508068);
  assert_approx(probabilities[1], 0.2306153779716283);
  assert_approx(probabilities[2], 0.3060480565775272);
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

  test_count_no_variants();
  test_count_one_variant();
  test_count_two_variants();
  test_count_three_variants();

  test_prob_b_beats_a();
  test_prob_c_beats_ab();
  test_prob_d_beats_abc();

  test_prob_1_beats_2();
  test_prob_1_beats_23();

  return 0;
}
