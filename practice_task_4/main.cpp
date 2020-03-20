// Группа солдат-новобранцев прибыла в армейскую часть N666. После знакомства прапорщиком стало очевидно, что от работ на кухне по очистке картофеля спасти солдат может только чудо.
// Прапорщик, будучи не в состоянии запомнить фамилии, пронумеровал новобранцев от 1 до N. После этого он велел им построиться по росту (начиная с самого высокого). С этой несложной задачей могут справиться даже совсем необученные новобранцы, да вот беда, прапорщик уверил себя, что знает про некоторых солдат, кто из них кого выше, и это далеко не всегда соответствует истине.
// После трех дней обучения новобранцам удалось выяснить, что знает (а точнее, думает, что знает) прапорщик. Помогите им, используя эти знания, построиться так, чтобы товарищ прапорщик остался доволен.
// Входные данные:
// Сначала на вход программы поступают числа N и M (1 < N <= 100, 1 <= M <= 5000) – количество солдат в роте и количество пар солдат, про которых прапорщик знает, кто из них выше. Далее идут эти пары чисел A и B по одной на строке (1 <= A,B <= N), что означает, что, по мнению прапорщика, солдат A выше, чем B. Не гарантируется, что все пары чисел во входных данных различны.
// Выходные данные:
// В первой строке выведите "Yes" (если можно построиться так, чтобы прапорщик остался доволен) или "No" (если нет). После ответа "Yes" на следующей строке выведите N чисел, разделенных пробелами, - одно из возможных построений.
// Примеры:
// входные данные:
// 4 5
// 1 2
// 2 3
// 3 4
// 1 4
// 4 1
// выходные данные:
// No
//
//
//  Created by Дмитрий Рябовский on 20.03.2020.
//  Copyright © 2020 Дмитрий Рябовский. All rights reserved.
//

#include <iostream>
#include <vector>
#include <list>
#include <queue>


using namespace std;


int getInt(int bottom, int top, string message) {
    int p;
    cout << message;
    while (!(cin >> p) || (p < bottom) || (p > top)) {
        cout << "Ввод некорректен. Повторите ввод: ";
        cin.clear();
        while (cin.get() != '\n');
    }
    cin.clear();
    while (cin.get() != '\n');
    cout << endl;
    return p;
}

int** getPairs(int n, int m) {
    int** pairs = new int*[m];
    for (int i = 0; i < m; i++)
        pairs[i] = new int[m];
    for (int i = 0; i < m; i++) {
        cout << "Введите поочередно номера двух солдат(A B), про которых прапорщик знает, кто из них выше(A>B): " << endl;
        pairs[i][0] = getInt(1, n, "");
        pairs[i][1] = getInt(1, n, "");
        while (pairs[i][1] == pairs[i][0]) {
            cout << "Ошибка ввода: A != B, повторите ввод B: ";
            pairs[i][1] = getInt(1, n, "");
        }
    }
    return pairs;
}

int searchIndexPair(vector<pair<int,vector<int>>> graph, int sElem) {
    int i = 0; for (pair<int,vector<int>> elem : graph) {
        if (elem.first == sElem)
            return i;
        i++;
    }
    return -1;
}

void addGraphVertices(vector<pair<int,vector<int>>>& graph, int a, int b) {
    int indexA = searchIndexPair(graph, a);
    int indexB = searchIndexPair(graph, b);
    if (indexB == -1) {
        vector<int> ways;
        pair<int, vector<int>> elem(b,ways);
        indexB = (int)graph.size();
        graph.push_back(elem);
    }
    if (indexA == -1) {
        graph.at(0).second.push_back(a);
        vector<int> ways(1,b);
        pair<int, vector<int>> elem(a,ways);
        graph.push_back(elem);
        return;
    }
    for (int way : graph.at(indexA).second) {
        if (way == b)
            return;
    }
    graph.at(indexA).second.push_back(b);
}

bool searchLoop(vector<pair<int,vector<int>>> graph, bool painted[], int currentIndex) {
    if (painted[currentIndex])
        return true;
    painted[currentIndex] = true;
    for (int way : graph.at(currentIndex).second) {
        if (searchLoop(graph, painted, searchIndexPair(graph, way)))
            return true;
    }
    painted[currentIndex] = false;
    return false;
}

bool searchLoop(vector<pair<int,vector<int>>> graph) {
    bool painted[graph.size()];
    for (int i = 0; i < graph.size(); i++)
        painted[i] = 0;
    return searchLoop(graph, painted, 0);
}

auto searchNumberIter(list<int> &list, int sElem) {
    auto iter = list.cbegin();
    while(*iter != sElem)
        iter++;
    return iter;
}

bool checkCorrectness(list<int> l, int a, int b) {
    for (int el : l) {
        if (el == a)
            return true;
        if (el == b)
            return false;
    }
    return false;
}

void printRanking(vector<pair<int,vector<int>>> graph, int n) {
    list<int> ranking;
    for (int i = 0; i <= n; i++)
        ranking.push_back(i);
    queue<int> bfsQueue;
    bfsQueue.push(0);
    while (!bfsQueue.empty()) {
        int currentIndex = searchIndexPair(graph, bfsQueue.front());
        for (int way : graph.at(currentIndex).second) {
            bfsQueue.push(way);
            if (!checkCorrectness(ranking, bfsQueue.front(), way)) {
                auto wayNumberIter = searchNumberIter(ranking, way);
                ranking.erase(wayNumberIter);
                auto currentNumberIter = searchNumberIter(ranking, bfsQueue.front());
                ranking.insert(++currentNumberIter, way);
            }
        }
        bfsQueue.pop();
    }
    ranking.pop_front();
    for (int el : ranking)
        cout << el << " ";
}


int main(int argc, const char * argv[]) {
    int n = getInt(2, 100, "Введите N - количество солдат в роте(1<N<=100): ");
    int m = getInt(1, 5000, "Введите M - количество пар солдат, про которых прапорщик знает, кто из них выше(1<=M<=5000): ");
    int** pairs = new int*[2];
    pairs = getPairs(n, m);
    vector<pair<int,vector<int>>> graphVertices;
    pair<int, vector<int>> root(0,NULL);
    graphVertices.push_back(root);
    for (int i = 0; i < m; i++) {
        addGraphVertices(graphVertices, pairs[i][0], pairs[i][1]);
    }
    /*
    for (pair<int,vector<int>> elem : graphVertices) {
        cout << elem.first << " | ";
        for(int way : elem.second)
            cout << way << " ";
        cout << endl;
    }
     */
    if (searchLoop(graphVertices)) {
        cout << "No" << endl;
    } else {
        cout << "Yes  ";
        printRanking(graphVertices, n);
    }
    return 0;
}
