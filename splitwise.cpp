#include <iostream>
#include <queue>

using namespace std;

// Comparator that will be used to make priority_queue
// containing pair of integers maxHeap Comparison is based
// on second entry in the pair which represents cash
// credit/debit
struct AscCmp
{
    bool operator()(const pair<int, int> &p1,
                    const pair<int, int> &p2)
    {
        return p1.second < p2.second;
    }
};

// Comparator that will be used to make priority_queue
// containing pair of integers minHeap Comparison is based
// on second entry in the pair which represents cash
// credit/debit
struct DscCmp
{
    bool operator()(const pair<int, int> &p1,
                    const pair<int, int> &p2)
    {
        return p1.second > p2.second;
    }
};

class Solution
{
private:
    // Below priority queues will be used as min_heap (minQ)
    // and max_heap (maxQ) First entry in the pair will be
    // person id Second entry in the pair will be cash
    // debit/credit for the same person
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   DscCmp>
        minQ;
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   AscCmp>
        maxQ;

    // This function will fill in minQ and maxQ in such a
    // way that maxQ will have only positive value. minQ
    // will have only negative value amount is taken as
    // input. amount[i] => cash credit/debit to/from person
    // i amount[i] == 0 will be ignored as no credit/debit
    // is left.
    void constructMinMaxQ(const vector<int> &amount)
    {
        for (int i = 0; i < amount.size(); ++i)
        {
            if (amount[i] == 0)
                continue;
            if (amount[i] > 0)
            {
                maxQ.emplace(i, amount[i]);
            }
            else
            {
                minQ.emplace(i, amount[i]);
            }
        }
    }

    // This function will iterate over minQ and maxQ until
    // empty. It will fetch max credit and min debit. If sum
    // of both is not equal to zero, then push remaining
    // credit or debit back to the required queue. At the
    // end of the loop, print result
    void solveTransaction()
    {
        while (!minQ.empty() && !maxQ.empty())
        {
            pair<int, int> maxCreditEntry = maxQ.top();
            maxQ.pop();

            pair<int, int> maxDebitEntry = minQ.top();
            minQ.pop();

            // Get the remaining transaction value by adding
            // credit and debit cash
            int transaction_val = maxCreditEntry.second + maxDebitEntry.second;

            int debtor = maxDebitEntry.first,
                creditor = maxCreditEntry.first,
                owed_amount = 0;

            if (transaction_val == 0)
            {
                // credit and debit cash for this person is
                // cancelled out
                owed_amount = maxCreditEntry.second;
            }
            else if (transaction_val < 0)
            {
                // transaction_val is the debit cash which
                // is remaining, so push to the queue for
                // further processing
                owed_amount = maxCreditEntry.second;
                minQ.emplace(maxDebitEntry.first,
                             transaction_val);
            }
            else
            {
                // transaction_val is the credit cash which
                // is remaining, so push to the queue for
                // further processing
                owed_amount = -(maxDebitEntry.second);
                maxQ.emplace(maxCreditEntry.first,
                             transaction_val);
            }

            // Print result
            cout << "Person " << debtor + 1 << " pays "
                 << owed_amount << " to Person " << creditor + 1
                 << endl;
        }
    }

public:
    void minCashFlow(const vector<vector<int>> &graph)
    {
        int n = graph.size();

        // Calculate the cash to be credited/debited to/from
        // each person and store in vector amount
        vector<int> amount(n, 0);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                int diff = graph[j][i] - graph[i][j];
                amount[i] += diff;
            }
        }

        // Fill in both queues minQ and maxQ using amount
        // vector
        constructMinMaxQ(amount);

        // Solve the transaction using minQ, maxQ and amount
        // vector
        solveTransaction();
    }
};

int main()
{
    // Input the number of group members
    cout << "Enter number of members in group:" << endl;
    int n;
    cin >> n;

    // Create a 2D vector (graph) to store debts between members
    vector<vector<int>> graph(n, vector<int>(n, 0));

    // Input the number of debts (transactions)
    cout << "Enter number of debts:" << endl;
    int m;
    cin >> m;

    // Input details of each debt: borrower, giver, and amount
    cout << "Enter debts where the first number is the borrower, second is the giver, and third is the amount:" << endl;
    for (int i = 0; i < m; i++)
    {
        int borrower, giver, amount;
        cin >> borrower >> giver >> amount;

        // Update the graph: add the amount to the borrower's debt to the giver
        graph[borrower - 1][giver - 1] += amount;
    }

    // Example of a predefined graph
    /*
    vector<vector<int>> graph = {
        {0, 1000, 2000},
        {0, 0, 5000},
        {0, 0, 0},
    };
    */

    // Output the simplified debt settlement
    cout << "Simplified Debt Settlement:" << endl;

    // Create an instance of Solution and call minCashFlow to settle debts
    Solution S;
    S.minCashFlow(graph);

    return 0;
}