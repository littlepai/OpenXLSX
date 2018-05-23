#include <iostream>
#include <chrono>
#include <iomanip>
#include "../src/OpenXLSX.h"

using namespace std;
using namespace OpenXLSX;

template<typename T>
unsigned long WriteTest(T value, unsigned long rows, unsigned long columns, int repetitions);

int main()
{
    WriteTest("Hello, OpenXLSX!", 1000, 1000, 10);
    return 0;
}

template<typename T>
unsigned long WriteTest(T value, unsigned long rows, unsigned long columns, int repetitions)
{
    std::cout << "Populating \"" << value << "\" to " << rows << " x " << columns << " cells:" << endl;

    unsigned long totTime = 0;
    unsigned long minTime = 0;
    for (int i = 1; i <= repetitions; i++) {

        OpenXLSX::XLDocument doc;
        doc.CreateDocument("Benchmark.xlsx");
        auto wks = doc.Workbook()->Worksheet("Sheet1");
        wks->Cell(rows, columns)->Value()->Set(1);

        auto start = chrono::steady_clock::now();
        auto arange = wks->Range();
        for (auto &iter : arange) {
            iter.Value()->Set(value);
        }

        auto end = chrono::steady_clock::now();
        totTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(minTime == 0 || minTime > std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count())
            minTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if (i >= repetitions) doc.SaveDocument();
        doc.CloseDocument();
        cout << "Run #" << i << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << endl;
    }

    totTime = totTime / repetitions;
    cout << "Average time: " << totTime << " milliseconds" << endl;
    cout << "Average cells per second: " << 1000000.0/totTime*1000 << endl;
    cout << "Minimum time: " << minTime << " milliseconds" << endl;
    cout << "Maximum cells per second: " << 1000000.0/minTime*1000 << endl << endl;

    return 0;
}

void myTest()
{
    XLDocument doc;
    doc.CreateDocument("./MyTest.xlsx");
    auto wks = doc.Workbook()->Worksheet("Sheet1");

    *wks->Cell("A1")->Value() = 3.14159;
    *wks->Cell("B1")->Value() = 42;
    *wks->Cell("C1")->Value() = "Hello OpenXLSX!";
    *wks->Cell("D1")->Value() = true;

    auto A1 = wks->Cell("A1")->Value()->Get<long double>();
    auto B1 = wks->Cell("B1")->Value()->Get<unsigned int>();
    auto C1 = wks->Cell("C1")->Value()->Get<std::string_view>();
    auto D1 = wks->Cell("D1")->Value()->Get<bool>();

    cout << "Cell A1: " << A1 << endl;
    cout << "Cell B1: " << B1 << endl;
    cout << "Cell C1: " << C1 << endl;
    cout << "Cell D1: " << D1 << endl;

    doc.SaveDocument();
}



void openLarge() {

    auto start = chrono::steady_clock::now();

    OpenXLSX::XLDocument doc;
    doc.OpenDocument("./Large.xlsx");
    auto wks = doc.Workbook()->Worksheet("Sheet1");
    wks->Export("./Profiles.csv");

    /*
    for (int i = 1; i <= 1000; ++i) {
        cout << setw(8) << i << ": ";
        cout << setw(10) << wks->Cell(i, 1)->Value()->AsString() << " ";
        cout << setw(38) << wks->Cell(i, 2)->Value()->AsString() << " ";
        cout << setw(38) << wks->Cell(i, 3)->Value()->AsString() << " ";
        cout << setw(38) << wks->Cell(i, 4)->Value()->AsString() << " ";
        cout << setw(38) << wks->Cell(i, 5)->Value()->AsString() << endl;
    } */

    doc.CloseDocument();

    auto end = chrono::steady_clock::now();
    cout << "Time to open file and export ~500k rows: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << endl;

}

void speedTest() {
    unsigned long time = 0;
    int numIter = 10;

    for (int i = 1; i <= numIter; i++) {

        OpenXLSX::XLDocument doc;
        doc.CreateDocument("SpeedTest.xlsx");
        auto wks = doc.Workbook()->Worksheet("Sheet1");
        wks->Cell(1000, 1000)->Value()->Set(1);

        auto start = chrono::steady_clock::now();

        auto arange = wks->Range();
        for (auto &iter : arange) {
            iter.Value()->Set("Hello OpenXLSX!");
        }

        auto end = chrono::steady_clock::now();

        time += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if (i >= numIter) doc.SaveDocument();
        doc.CloseDocument();
        cout << "Run #" << i << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << endl;
    }

    time = time / numIter;
    cout << "Average time: " << time << " milliseconds" << endl;
    cout << "Average cells per second: " << 1000000.0/time*1000 << endl;

}

