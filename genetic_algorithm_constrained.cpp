// Markus Buchholz
// g++ genetic_algorithm_constrained.cpp -O3 -o genetic
// https://en.wikipedia.org/wiki/Test_functions_for_optimization

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <math.h>
#include <random>

//--------P---------------------------------------------------------------------

float x1min = 1.0;
float x1max = 5.0;

float x2min = 1.0;
float x2max = 5.0;

float x3min = 1.0;
float x3max = 5.0;

float x4min = 1.0;
float x4max = 5.0;

//--------------------------------------------------------------------------------
int DIM = 4;
int CHROMOSOMES = 10000;
float ERROR = 0.00001;

int ELITISM = 20;
int CROSSOVER = 40;
int MUTATION = 40;

//--------------------------------------------------------------------------------

struct Pos
{

    float x1;
    float x2;
    float x3;
    float x4;
};

//--------------------------------------------------------------------------------

float generateRandom()
{

    std::random_device engine;
    std::mt19937 gen(engine());
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution(gen);
}

//--------------------------------------------------------------------------------

float generateRandomX()
{

    std::random_device engine;
    std::mt19937 gen(engine());
    std::uniform_real_distribution<float> distribution(-1.0, 1.0);
    return distribution(gen);
}
//--------------------------------------------------------------------------------

float valueGenerator(float low, float high)
{

    return low + generateRandom() * (high - low);
}

//--------------------------------------------------------------------------------

std::vector<float> function(std::vector<Pos> pos)
{

    std::vector<float> funcValue;
    float penalty1;
    float penalty2;

    for (auto &ii : pos)
    {

        if (ii.x1 * ii.x2 * ii.x3 * ii.x4 < 25)
        {
            penalty1 = 3;
        }
        else
        {
            penalty1 = 0;
        }

        if (ii.x1 * ii.x1 + ii.x1 * ii.x2 + ii.x3 * ii.x3 + ii.x4 * ii.x4 != 40)
        {
            penalty1 = 10;
        }
        else
        {
            penalty1 = 0;
        }

        funcValue.push_back(ii.x1 * ii.x2 * (ii.x1 + ii.x2 + ii.x3) + ii.x3 + penalty1 + penalty2);
    }

    return funcValue;
}

//--------------------------------------------------------------------------------

Pos positionUpdateCheck(Pos actualPos)
{

    Pos Pnew = actualPos;

    if (Pnew.x1 < x1min)
    {
        Pnew.x1 = x1min;
    }

    if (Pnew.x1 > x1max)
    {
        Pnew.x1 = x1max;
    }

    if (Pnew.x2 < x2min)
    {
        Pnew.x2 = x2min;
    }

    if (Pnew.x2 > x2max)
    {
        Pnew.x2 = x2max;
    }

    if (Pnew.x3 < x3min)
    {
        Pnew.x3 = x3min;
    }

    if (Pnew.x3 > x3max)
    {
        Pnew.x3 = x3max;
    }

    if (Pnew.x4 < x4min)
    {
        Pnew.x4 = x4min;
    }

    if (Pnew.x4 > x4max)
    {
        Pnew.x4 = x4max;
    }

    return Pnew;
}
//--------------------------------------------------------------------------------
int dimensionToUpdate()
{

    std::random_device engine;
    std::mt19937 gen(engine());
    std::uniform_int_distribution<int> distribution(0, DIM - 1);
    return distribution(gen);
}

//--------------------------------------------------------------------------------

Pos posMutation(Pos chromA, Pos chromB)
{

    int dominant = dimensionToUpdate();
    Pos Xnew;

    if (dominant == 0)
    {

        Xnew.x1 = chromA.x1 + generateRandomX() * (chromA.x1 - chromB.x1);
        Xnew.x2 = chromA.x2 + generateRandomX() * (chromA.x2 - chromB.x2);
        Xnew.x3 = chromA.x3 + generateRandomX() * (chromA.x3 - chromB.x3);
        Xnew.x4 = chromA.x4 + generateRandomX() * (chromA.x4 - chromB.x4);
    }
    else if (dominant == 1)
    {
        Xnew.x1 = chromB.x1 + generateRandomX() * (chromB.x1 - chromA.x1);
        Xnew.x2 = chromB.x2 + generateRandomX() * (chromB.x2 - chromA.x2);
        Xnew.x3 = chromB.x3 + generateRandomX() * (chromB.x3 - chromA.x3);
        Xnew.x4 = chromB.x4 + generateRandomX() * (chromB.x4 - chromA.x4);
    }

    return positionUpdateCheck(Xnew);
}

//--------------------------------------------------------------------------------

Pos posCrossover(Pos chromA, Pos chromB)
{

    int cross = dimensionToUpdate();
    Pos Xnew;

    if (cross == 0)
    {
        Xnew.x1 = chromA.x1;
        Xnew.x2 = chromA.x2;
        Xnew.x3 = chromB.x3;
        Xnew.x4 = chromB.x4;
    }
    else if (cross == 1)
    {
        Xnew.x1 = chromB.x1;
        Xnew.x2 = chromB.x2;
        Xnew.x3 = chromA.x3;
        Xnew.x4 = chromA.x4;
    }
    else if (cross == 2)
    {
        Xnew.x1 = chromB.x1;
        Xnew.x2 = chromA.x2;
        Xnew.x3 = chromA.x3;
        Xnew.x4 = chromB.x4;
    }
    else if (cross == 3)
    {
        Xnew.x1 = chromB.x1;
        Xnew.x2 = chromA.x2;
        Xnew.x3 = chromA.x3;
        Xnew.x4 = chromB.x4;
    }

    return positionUpdateCheck(Xnew);
}

//--------------------------------------------------------------------------------

std::vector<Pos> initPosXY()
{

    std::vector<Pos> pos;

    for (int ii = 0; ii < CHROMOSOMES; ii++)
    {

        pos.push_back({valueGenerator(x1min, x1max), valueGenerator(x2min, x2max), valueGenerator(x3min, x3max), valueGenerator(x4min, x4max)});
    }

    return pos;
}

//--------------------------------------------------------------------------------

Pos newPosXY()
{

    Pos pos = {valueGenerator(x1min, x1max), valueGenerator(x2min, x2max), valueGenerator(x3min, x3max), valueGenerator(x4min, x4max)};

    return pos;
}

//-------------------------------------------------------------------------------
bool compareMin(std::pair<Pos, float> a, std::pair<Pos, float> b)
{

    return a.second < b.second;
}

//-------------------------------------------------------------------------------

// min
std::tuple<std::vector<Pos>, std::vector<Pos>, std::vector<Pos>> evolutionSet(std::vector<Pos> positions, std::vector<float> func)
{

    std::vector<std::pair<Pos, float>> best;
    std::vector<Pos> rest;

    std::vector<Pos> elit;
    std::vector<Pos> cross;
    std::vector<Pos> mutation;

    for (int ii = 0; ii < func.size(); ii++)
    {

        best.push_back(std::pair<Pos, float>(positions[ii], func[ii]));
    }

    std::sort(best.begin(), best.end(), compareMin);

    for (int ii = 0; ii < best.size(); ii++)
    {
        auto pos = std::get<0>(best[ii]);
        if (ii < ELITISM)
        {

            elit.push_back(pos);
        }
        else if (ii > ELITISM)
        {
            rest.push_back(pos);
        }
    }

    for (int ii = 0; ii < rest.size(); ii++)
    {

        if (ii < CROSSOVER)
        {
            cross.push_back(rest[ii]);
        }
        else if (ii > CROSSOVER)
        {
            mutation.push_back(rest[ii]);
        }
    }
    return std::make_tuple(elit, cross, mutation);
}

//-------------------------------------------------------------------------------

int chooseChromosome(int actual)
{

    std::random_device engine;
    std::uniform_int_distribution<int> distribution(0, CHROMOSOMES);

    int r = -1;

    do
    {

        r = distribution(engine);

    } while (r == actual);

    return r;
}

//-------------------------------------------------------------------------------

std::tuple<bool, Pos> checkCondition(std::vector<Pos> positions)
{
    Pos pos_null;
    float error = 1.0;

    for (auto &ii : positions)
    {

        float st1 = ii.x1 * ii.x2 * ii.x3 * ii.x4;
        float st2 = ii.x1 * ii.x1 + ii.x2 * ii.x2 + ii.x3 * ii.x3 + ii.x4 * ii.x4;

        if ((st1 >= 25.0f - ERROR) && (st2 >= 40.0f - ERROR) && (st2 <= 40.0f + ERROR))
        {
            return std::make_tuple(true, ii);
        }
    }
    return std::make_tuple(false, pos_null);
}
//-------------------------------------------------------------------------------

Pos runGA()
{

    std::vector<Pos> currentPositions = initPosXY();
    std::vector<float> valueFunction;
    Pos optimalPos;
    bool flag = false;

    while (!flag)

    {

        std::vector<float> currentValueFunction = function(currentPositions);
        auto evoSet = evolutionSet(currentPositions, currentValueFunction);
        currentPositions.erase(currentPositions.begin(), currentPositions.end());

        std::vector<Pos> elite = std::get<0>(evoSet);
        std::vector<Pos> cross = std::get<1>(evoSet);
        std::vector<Pos> mutation = std::get<2>(evoSet);

        std::vector<Pos> evoPositions(elite);

        for (int jj = 0; jj < cross.size(); jj++)
        {

            int chromB_pos = chooseChromosome(jj);
            Pos newCross = posCrossover(cross[jj], cross[chromB_pos]);
            evoPositions.push_back(newCross);
        }

        for (int kk = 0; kk < mutation.size(); kk++)
        {

            int chromB_pos = chooseChromosome(kk);
            Pos newMutation = posMutation(mutation[kk], mutation[chromB_pos]);
            evoPositions.push_back(newMutation);
        }

        currentPositions = evoPositions;

        valueFunction = currentValueFunction;

        auto checkC = checkCondition(currentPositions);
        flag = std::get<0>(checkC);
        optimalPos = std::get<1>(checkC);
    }

    return optimalPos;
}

//-------------------------------------------------------------------------------

int main()
{

    Pos solution = runGA();
    std::cout << "solution for optimization problem is found!"
              << "\n";
    std::cout << "x1= " << solution.x1 << ", x2= " << solution.x2 << ", x3= " << solution.x3 << ", x4= " << solution.x4 << "\n";
}
