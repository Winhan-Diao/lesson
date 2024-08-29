#pragma once
#include "include_headers.h"
#include "quadratic_solution.h"
#include "value_types.h"

template<class CharT1, class Traits1, class CharT2, class Traits2>
void solveStream(std::basic_istream<CharT1, Traits1>& inputStream, std::basic_ostream<CharT2, Traits2>& outputStream) {
    DecaiableDoubleComplex a, b, c;
    inputStream >> a >> b >> c;
    auto [ans1, ans2] = quadraticSolution(a, b, c);
    outputStream << "Answer: " << ans1 << ", " << ans2 << "\r\n";
}

void solveContainer() {

}

struct Solver {
protected:
    DecaiableDoubleComplex a{}, b{}, c{}, ans1{}, ans2{};
public:
    virtual void preIn() {}
    virtual void input() = 0;
    virtual void postInPreOut() = 0;
    virtual void output() = 0;
    virtual void postOut() {}
    void operator() () {
        preIn();
        input();
        postInPreOut();
        output();
        postOut();
    }
    void solve() {
        std::tie(ans1, ans2) = quadraticSolution(a, b, c);
    }
};

template <class CharT1, class Traits1, class CharT2, class Traits2>
struct StreamSolver: public Solver {
protected:
    std::basic_istream<CharT1, Traits1>& inputStream;
    std::basic_ostream<CharT2, Traits2>& outputStream;
public:
    StreamSolver(std::basic_istream<CharT1, Traits1>& inputStream, std::basic_ostream<CharT2, Traits2>& outputStream): inputStream(inputStream), outputStream(outputStream) {}
    void input() override {
        inputStream >> a >> b >> c;
    }
    void postInPreOut() override {
        solve();
    }
    void output() override {
        outputStream << ans1 << ' ' << ans2 << "\r\n";
    }
};

template <class CharT1, class Traits1, class CharT2, class Traits2>
struct VerboseStreamSolver: public StreamSolver<CharT1, Traits1, CharT2, Traits2> {
    VerboseStreamSolver(std::basic_istream<CharT1, Traits1>& inputStream, std::basic_ostream<CharT2, Traits2>& outputStream): StreamSolver<CharT1, Traits1, CharT2, Traits2>(inputStream, outputStream) {}
    void preIn() override {
        this->outputStream << "= = = = = = = = = = = = = = =" << "\r\n"
                     << "解决 Ax² + Bx + C = 0." << "\r\n"
                     << "请依次输入A、B、C." << "\r\n"
                     << "支持格式1.r; 2.(r, i)." << "\r\n";
    }
    void postInPreOut() override {
        this->outputStream << "将计算" << this->a << "x² + " << this->b << "x + " << this->c << " = 0." << "\r\n";
        this->solve();
    }
    void output() override {
        this->outputStream << "答案: (" <<this->ans1 << ", " << this->ans2 << ")." << "\r\n"
                     << "= = = = = = = = = = = = = = =" << "\r\n";
    }
};

int main();