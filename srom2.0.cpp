#include "Header.h"

using namespace std;
using namespace chrono;

FieldGF::FieldGF() {
    data.fill(0);
}

FieldGF::FieldGF(const std::array<int, array_size>& coefficients) {
    data = coefficients;
}

FieldGF FieldGF::zero() const {
    return FieldGF();
}

FieldGF FieldGF::one() const {
    std::array<int, array_size> oneCoefficients = { 0 };
    oneCoefficients[0] = 1;
    return FieldGF(oneCoefficients); 
}



FieldGF FieldGF::operator+(const FieldGF& other) const {
    FieldGF result;
    for (std::size_t i = 0; i < array_size; i++) {
        result.data[i] = (data[i] ^ other.data[i]);
    }
    return result;
}

FieldGF FieldGF::operator*(const FieldGF& other) const {
    FieldGF result;

    std::array<int, 2 * array_size - 1> temp;
    temp.fill(0);

    for (std::size_t i = 0; i < array_size; ++i) {
        for (std::size_t j = 0; j < array_size; ++j) {
            temp[i + j] ^= data[i] & other.data[j];
        }
    }

    std::array<int, 2 * array_size - 1> mod_poly{ 0 };
    mod_poly[0] = 1;
    mod_poly[1] = 1;
    mod_poly[2] = 1;
    mod_poly[4] = 1;
    mod_poly[179] = 1;

    for (std::size_t i = 2 * array_size - 2; i >= array_size; --i) {
        if (temp[i] != 0) {
            for (std::size_t j = 0; j < array_size; ++j) {
                temp[i - array_size + j] ^= mod_poly[j];
            }
        }
    }

    for (std::size_t i = 0; i < array_size; ++i) {
        result.data[i] = temp[i];
    }

    return result;
}

FieldGF FieldGF::toSquare(const FieldGF& A) const {
    FieldGF result;
    result = A * A;
    return result;
}

FieldGF FieldGF::pow(const FieldGF& power) const {
    FieldGF result(one()); 
    FieldGF base = *this;

    for (int i = 0; i < array_size; ++i) {
        if (power.data[i] != 0) {
            result = result * base;
        }
        base = toSquare(base);
    }

    return result;
}

FieldGF FieldGF::trace() const {
    FieldGF temp = *this;
    FieldGF trace;
    trace.data.fill(0);

    for (int i = 0; i < array_size * 8 - 1; ++i) {
        trace = trace + temp;
        temp = toSquare(temp);
    }

    return FieldGF({ trace.data[array_size - 1] & 1 });
}

FieldGF FieldGF::inverse() const {
    if (data == std::array<int, array_size>{0}) {
        throw std::runtime_error("inverse for zero?");
    }

    FieldGF result(*this);
    FieldGF temp(*this);

    for (int i = 0; i < array_size - 2; ++i) {
        temp = toSquare(temp);
        result = result * temp;
    }

    result = toSquare(result);

    return result;
}

std::string FieldGF::toString() const {
    std::string result;
    for (int i = array_size - 1; i >= 0; i--) {
        result += std::to_string(data[i]);
    }
    return result;
}

FieldGF FieldGF::fromString(const std::string& str) {

    FieldGF result;
    for (std::size_t i = 0; i < array_size && i < str.length(); ++i) {
        result.data[array_size-i-1] = str[i]  - '0';
    }
    return result;
}

std::string generateRandomBinaryString() {
    std::string result;
    for (std::size_t i = 0; i < array_size-1; ++i) {
        result += std::to_string(rand() % 2);
    }
    return result;
}


//template <typename Func>
//double measureTime(Func&& func) {
//    auto start = high_resolution_clock::now();
//    func();
//    auto end = high_resolution_clock::now();
//    return duration_cast<nanoseconds>(end - start).count();
//}
//
//template <typename T>
//void runTests(T length) {
//    double AddTime = 0.0;
//    double MultTime = 0.0;
//    double PowTime = 0.0;
//    double InvTime = 0.0;
//    double TrTime = 0.0;
//
//    for (int i = 0; i < NUM_TESTS; i++)
//    {
//        FieldGF num1(generateRandomBinaryString());
//        FieldGF num2(generateRandomBinaryString());
//        FieldGF num3(generateRandomBinaryString());
//
//        AddTime += measureTime([&] {FieldGF resultAdd = num1 + num2; });
//        MultTime += measureTime([&] {FieldGF resultAdd = num1 * num2; });
//        PowTime += measureTime([&] {FieldGF resultAdd = num1.pow(num2); });
//        InvTime += measureTime([&] {FieldGF resultAdd = num1.inverse(); });
//        TrTime += measureTime([&] {FieldGF resultAdd = num1.trace(); });
//    }
//    double AvgAddTime = 0.0;
//    double AvgMultTime = 0.0;
//    double AvgPowTime = 0.0;
//    double AvgInvTime = 0.0;
//    double AvgTrTime = 0.0;
//
//    double AvgAddTime = AddTime / NUM_TESTS;
//    double AvgMultTime = MultTime / NUM_TESTS;
//    double AvgPowTime = PowTime / NUM_TESTS;
//    double AvgInvTime = InvTime / NUM_TESTS;
//    double AvgTrTime = TrTime / NUM_TESTS;
//
//    cout << "Average Addition Time: " << AvgAddTime << " nanoseconds" << endl;
//    cout << "Average Multiplication Time: " << AvgMultTime << " nanoseconds" << endl;
//    cout << "Average Power Time: " << AvgPowTime << " nanoseconds" << endl;
//    cout << "Average Inversion Time: " << AvgInvTime << " nanoseconds" << endl;
//    cout << "Average Trace Time: " << AvgTrTime << " nanoseconds" << endl;
//    cout << "\t" << endl;
//
//}

int main() {
    FieldGF A;
    FieldGF B;
    FieldGF C;
    

 //   std::array<int, array_size> coefficientsA = { 1, 1 }; 
 //   std::array<int, array_size> coefficientsB = { 1, 0, 1 };
 //   std::array<int, array_size> coefficientsC = { 1, 1, 1 };

 //   A = FieldGF(coefficientsA);
 //   B = FieldGF(coefficientsB);
 //   C = FieldGF(coefficientsC);

    /*A = A.fromString("01001001011110110000001011110011010100101110111001100010100011001010010011011100010010100110100100110001111010011100111111010101011010110111100001101011111100111011001000010111111");
    B = B.fromString("11000111100011110111011001110100111100111011111100101110000111100011010011010110110001001100011110111011110011001011110100111000110101011110010100100000011010011011000100001010111");
    C = C.fromString("11000000101000011101010010000011101100011101111110111010111010100000100001101011101100010000000101100011000001110110010110101011001011101011110000011001111110100101111011110011011");
    
    std::cout << "A: "<< A.toString() << std::endl;
    std::cout << "B: "<< B.toString() << std::endl;
    std::cout << "C: "<< C.toString() << std::endl;*/


    /*A = A.fromString(generateRandomBinaryString());
    B = B.fromString(generateRandomBinaryString());
    C = C.fromString(generateRandomBinaryString());

    std::cout << "A: " << A.toString() << std::endl;
    std::cout << "B: " << B.toString() << std::endl;
    std::cout << "C: " << C.toString() << std::endl;

    
    FieldGF sum = A + B;
    std::cout << "A + B = " << sum.toString() << std::endl;

    FieldGF product = A * B;
    std::cout << "A * B = " << product.toString() << std::endl;

    FieldGF square = A.toSquare(A);
    std::cout << "A * A = " << square.toString() << std::endl;

    FieldGF power = A.pow(C);
    std::cout << "A^C = " << power.toString() << std::endl;\

    FieldGF tr = A.trace();
    std::cout << "Trace(A) " << tr.toString() << std::endl;

    FieldGF inv = A.inverse();
    std::cout << "Inverse(A) " << inv.toString() << std::endl;*/

    const int iterations = 100;

    srand(static_cast<unsigned>(time(nullptr)));

    
    double addTime = 0.0;
    double multiplyTime = 0.0;
    double powTime = 0.0;
    double traceTime = 0.0;
    double inverseTime = 0.0;

    for (int i = 0; i < iterations; ++i) {
        
        FieldGF a = FieldGF::fromString(generateRandomBinaryString());
        FieldGF b = FieldGF::fromString(generateRandomBinaryString());

        
        auto start = std::chrono::high_resolution_clock::now();
        FieldGF resultAdd = a + b;
        auto end = std::chrono::high_resolution_clock::now();
        addTime += std::chrono::duration<double>(end - start).count();

        
        start = std::chrono::high_resolution_clock::now();
        FieldGF resultMultiply = a * b;
        end = std::chrono::high_resolution_clock::now();
        multiplyTime += std::chrono::duration<double>(end - start).count();

        
        start = std::chrono::high_resolution_clock::now();
        FieldGF resultPow = a.pow(b);
        end = std::chrono::high_resolution_clock::now();
        powTime += std::chrono::duration<double>(end - start).count();

        
        start = std::chrono::high_resolution_clock::now();
        FieldGF resultTrace = a.trace();
        end = std::chrono::high_resolution_clock::now();
        traceTime += std::chrono::duration<double>(end - start).count();

        
        start = std::chrono::high_resolution_clock::now();
        FieldGF resultInverse = a.inverse();
        end = std::chrono::high_resolution_clock::now();
        inverseTime += std::chrono::duration<double>(end - start).count();
    }

    double avgAddTime = addTime / iterations;
    double avgMultiplyTime = multiplyTime / iterations;
    double avgPowTime = powTime / iterations;
    double avgTraceTime = traceTime / iterations;
    double avgInverseTime = inverseTime / iterations;

    std::cout << "Average Time for Addition: " << avgAddTime << " seconds\n";
    std::cout << "Average Time for Multiplication: " << avgMultiplyTime << " seconds\n";
    std::cout << "Average Time for Power: " << avgPowTime << " seconds\n";
    std::cout << "Average Time for Trace: " << avgTraceTime << " seconds\n";
    std::cout << "Average Time for Inverse: " << avgInverseTime << " seconds\n";

    return 0;
}