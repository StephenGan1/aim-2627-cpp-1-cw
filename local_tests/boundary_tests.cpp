
#define main public_main_q1
#include "../src/main/A1_q1.cpp"
#undef main

#define main public_main_q2
#include "../src/main/A1_q2.cpp"
#undef main

#define main public_main_q3
#include "../src/main/A1_q3.cpp"
#undef main

#define main public_main_q4
#include "../src/main/A1_q4.cpp"
#undef main

#define main public_main_q5
#include "../src/main/A1_q5.cpp"
#undef main

#define main public_main_q6
#include "../src/main/A1_q6.cpp"
#undef main

#define main public_main_q7
#include "../src/main/A1_q7.cpp"
#undef main

#define main public_main_q8
#include "../src/main/A1_q8.cpp"
#undef main

#define main public_main_b1
#include "../src/main/A1_b1.cpp"
#undef main

#define main public_main_b2
#include "../src/main/A1_b2.cpp"
#undef main

int main() {
    int failed = 0;
    int total = 0;

    auto check = [&](const char* name, bool ok) {
        ++total;
        std::cout << (ok ? "PASS " : "FAIL ") << name << '\n';
        if (!ok) ++failed;
    };

    check("Q1: HP boundaries",
        aim::q1::ratio(-5, 100) == 0 &&
        aim::q1::ratio(200, 100) == 100 &&
        aim::q1::ratio(1, 0) == -1);

    check("Q2: exact label matching",
        aim::q2::edit_labels(
            "cam2,cam", {{"cam", "imu"}}
        ) == "cam2,imu");

    check("Q3: threshold equality",
        aim::q3::alarms({2, 2}, 2, 4).empty() &&
        aim::q3::alarms({1, 2}, 1, 1)
            == std::vector<int>{1});

    auto command = aim::q4::execute({"ADD 5", "SET 8 x"});
    check("Q4: transactional rejection",
        command.value == 5 &&
        command.accepted == 1 &&
        command.rejected == 1);

    bool invalid_grid = false;
    try {
        (void)aim::q5::risk({"..", "."});
    } catch (const std::invalid_argument&) {
        invalid_grid = true;
    }
    check("Q5: invalid grid dimensions", invalid_grid);

    check("Q6: final ID tie-break",
        aim::q6::select_target(
            {{9, 3, 4, 10, true},
             {2, -3, -4, 10, true}}, 5
        ) == 2);

    auto fire = aim::q7::simulate(3, 100, 20, 10, {0});
    check("Q7: cooling without firing",
        fire.heat == 80 &&
        fire.ammo == 3 &&
        fire.shots == 0);

    auto summary = aim::q8::summarize(
        {{-1, 30}, {1, 30}, {1, 1001}}
    );
    check("Q8: invalid frame before duplicate",
        summary.valid == 1 &&
        summary.duplicate == 0 &&
        summary.invalid == 2 &&
        summary.total_hp == 30);

    int value = 77;
    bool min_ok = aim::b1::parse_i32("-2147483648", value)
        && value == std::numeric_limits<int>::min();

    int unchanged = 123;
    bool overflow_ok =
        !aim::b1::parse_i32("2147483648", unchanged)
        && unchanged == 123;

    check("B1: integer boundaries and overflow",
        min_ok && overflow_ok);

    int data[] = {3, 1, 2, 3};
    std::size_t n = aim::b2::compact(data, 4, 2, 3);

    check("B2: stable in-place compression",
        n == 3 &&
        data[0] == 3 &&
        data[1] == 2 &&
        data[2] == 3);

    std::cout << "BOUNDARY "
              << total - failed << "/" << total << '\n';

    return failed == 0 ? 0 : 1;
}
