#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <limits>
#include <iomanip>


using namespace std;
/**
VWAPer v0.1

// Read file in with the following format:
// [Stock],[Interval],[Volume Traded],[High],[Low]

// Calculate the total volume traded per Stock
// Calculate the total volume traded per Stock&Interval

// Write the total volume traded per Stock&Interval as a percentage of the total volume traded per Stock to stdout:
// [Stock],[Interval],[%Volume Traded]

// Write the delimiter '#' to stdout

// Write the maximum High and minimum Low for each Stock to stdout:
// [Stock],[Day High],[Day Low]

// example input:
VOD.L 1 100 184 183.7
BT.LN 1 300 449.4 448.2
VOD.L 2 25 184.1 182.4
BT.LN 2 900 449.8 449.5

// example output:
VOD.L,1,80
BT.LN,1,25
VOD.L,2,20
BT.LN,2,75
#
VOD.L,184.1,182.4
BT.LN,449.8,448.2

**/

//organise data in data structure
struct StockData {
    int interval;
    int volume;
    float high;
    float low;
};

// This class hold current Low and High each stock
// nCurUpr and nCurLwr set to the lowest and max float value respectively
class CUpperLower {
public:
    CUpperLower() : nCurLwr(numeric_limits<float>::max()), nCurUpr(numeric_limits<float>::lowest()) {}

    void add(float nHigh, float nLow) {
        if (nHigh > nCurUpr) {
            nCurUpr = nHigh;
        }
        if (nLow < nCurLwr) {
            nCurLwr = nLow;
        }
    }

    float nCurLwr;
    float nCurUpr;
};

int main(int argc, char* argv[]) {
    // Ensure there are enough arguments
    if(argc < 2) {
   	cerr << "Usage: " << argv[0] << " data_file.txt or version " << endl;
        return 1;
   } 
   if (argc > 1 && string("version") == argv[1])
    {
        cout << "VWAPer version 0.1" << endl;
        return 0;
    }
    
    // Open the file
    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error: Could not open file " << argv[1] << endl;
        return 1;
    }
    
    cout << "Reading file " << argv[1] << endl;

    map<string, vector<StockData>> stockMap;
    string line;
    int i = 1;

    // Read data from the file
    while (getline(file, line)) {
        istringstream iss(line);
        string stock;
        StockData data;

        // Parsing
        // if a line is not formatted as expected throw an error and continue to the next line
        if (!(iss >> stock >> data.interval >> data.volume >> data.high >> data.low)) {
            cerr << "WARNING: On the line number "<<i<<" set of data are not correctly formatted. Data were skipped"<< endl;
            continue;
        }

        stockMap[stock].push_back(data);
    }

    cout << "Calculating total volumes and high lows" << endl;

    map<string, int> totalVolumes;
    map<pair<string, int>, int> totalVolumesByInterval;
    map<string, CUpperLower> highLows;

    // Calculate total volumes, volumes per interval, and high/lows per stock
    for (const auto& itr : stockMap) 
    {
        const string& stock = itr.first;
        const vector<StockData>& dataList = itr.second;

        for (const auto& data : dataList) 
        {
            totalVolumes[stock] += data.volume;
            totalVolumesByInterval[{stock, data.interval}] += data.volume;
            highLows[stock].add(data.high, data.low);
        }
    }
     
    cout << "Volume in percentage" << endl;

    // Calculate and output the volume traded as a percentage of the total volume for each stock and interval
    for (const auto& itr : totalVolumesByInterval) 
    {
        const string& stock = itr.first.first;
        int interval = itr.first.second;
        int volume = itr.second;
        float percentage = (static_cast<float>(volume) / totalVolumes[stock]) * 100.0f;
        cout << stock << "," << interval << "," << percentage << endl;
    }

    // Output the delimiter
    cout << "#" << endl;

    // Output the maximum high and minimum low for each stock
    for (const auto& itr : highLows) {
        cout << itr.first << "," << itr.second.nCurUpr << "," << itr.second.nCurLwr << endl;
    }

    return 0;
}
