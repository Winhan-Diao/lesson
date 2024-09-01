#pragma once
#include "include_headers.h"
#include "value_types.h"
#include "quadratic_solution.h"

struct BadDistanceException: public std::exception {
    const char *what() const throw() override {
        return "Bad Distance Exception";
    }
};

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
        try {
            preIn();
            input();
            postInPreOut();
            output();
            postOut();
        } catch (BadDistanceException& e) {
            std::cerr << e.what() << "\r\n";        //error
        }
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

template <class InputIt, class OutputIt>
struct FlatContainerToPairSolver: public Solver {
    bool preInOnceFlag{true};
    bool logFlag;
    InputIt inTemp;
    OutputIt outTemp;
protected:
    InputIt inFirst;
    InputIt inLast;
    OutputIt outFirst;
public:
    FlatContainerToPairSolver(InputIt inFirst, InputIt inLast, OutputIt outFirst, bool logFlag = false): logFlag(logFlag), inTemp(inFirst), outTemp(outFirst), inFirst(inFirst), inLast(inLast), outFirst(outFirst) {}
    void preIn() override {
        if (preInOnceFlag) {
            preInOnceFlag = false;
            if ((inLast - inFirst) % 3 != 0) {
                throw BadDistanceException{};
            }
        }
    }
    void input() override {
        a = *inTemp;
        ++inTemp;
        b = *inTemp;
        ++inTemp;
        c = *inTemp;
        ++inTemp;
    } 
    void postInPreOut() override {
        solve();
    }
    void output() override {
        *outTemp = std::make_pair(ans1, ans2);
        if (logFlag) std::cout << "{" << "ans1: " << ans1 << "; ans2: " << ans2 << "}" << "\r\n"; 
        ++outTemp;
    }
    void postOut() override {
        if (inTemp >= inLast) return;
        this->operator()();
    }
};