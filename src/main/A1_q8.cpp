#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// Assessment 01 Q8; see 题面.pdf.
namespace aim::q8 {
struct Telemetry { int id; int hp; };
struct Summary { int valid; int duplicate; int invalid; long long total_hp; std::vector<int> ids; };

// BEGIN IMPLEMENTATION
// aim-cpp-cw-known

Summary summarize(const std::vector<Telemetry>& frames) {
    int valid = 0;
    int duplicate = 0;
    int invalid = 0;
    long long total_hp = 0;

    // 编号范围为 0~9999
    std::vector<bool> seen(10000, false);

    for (const auto& frame : frames) {

        // 第一步：验证数据范围
        if (frame.id < 0 || frame.id > 9999 ||
            frame.hp < -1000 || frame.hp > 1000) {
            ++invalid;
            continue;
        }

        // 第二步：检查重复
        if (seen[frame.id]) {
            ++duplicate;
            continue;
        }

        // 第三步：首次合法记录
        seen[frame.id] = true;
        ++valid;

        // 血量限制到 0~100
        int hp = std::clamp(frame.hp, 0, 100);
        total_hp += hp;
    }

    // 第四步：按照编号升序生成结果
    std::vector<int> ids;

    for (int id = 0; id < 10000; ++id) {
        if (seen[id]) {
            ids.push_back(id);
        }
    }

    return Summary{
        valid,
        duplicate,
        invalid,
        total_hp,
        ids
    };
}

// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q8;
int main() {
    try {
        { aim::q8::Summary r=summarize({{2,200},{1,-5},{2,10},{-1,30}}); CHECK(r.valid==2 && r.duplicate==1 && r.invalid==1 && r.total_hp==100 && (r.ids==std::vector<int>{1,2})); }
        { aim::q8::Summary r=summarize({{1,-1001},{1,30},{1,1001}}); CHECK(r.valid==1 && r.duplicate==0 && r.invalid==2 && r.total_hp==30 && (r.ids==std::vector<int>{1})); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
