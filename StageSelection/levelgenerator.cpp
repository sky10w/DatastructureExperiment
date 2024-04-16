#include "levelgenerator.h"

LevelGenerator::LevelGenerator(QObject *parent)
    : QObject{parent}
{

}


void LevelGenerator::generateLevel() {

    QList<lnode> nodes;
    QList<QPair<int, int>> edges;
    nodes.resize(9);

    for (int i = 0; i < 9; ++i) {
        nodes[i].position = QPoint(100,100);
        nodes[i].currentlevel = 0;
        nodes[i].leveltype = (rand() % (3-1+1))+ 1;
    }

    QList<int> permutation;
    for (int i = 0; i < nodes.size(); ++i) {
        permutation << i;
    }

    std::shuffle(permutation.begin(), permutation.end(),std::default_random_engine(time(NULL)));

    // 生成边
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, nodes.size() - 4);
    int branchIndex = dis(gen);

    // 根据分叉节点生成边
    for (int i = 1; i < nodes.size() - 2; ++i) {
        edges << qMakePair(i - 1, i);
        if (i == branchIndex) {
            edges << qMakePair(i, i + 2);
            edges << qMakePair(i, i + 1);
            i=i+2;
            edges << qMakePair(i - 1, i+1);
        }
    }
    edges << qMakePair(nodes.size() - 3, nodes.size() - 2)
          << qMakePair(nodes.size() - 2, nodes.size() - 1);

    // 生成节点
    for(int i = 0,branch = 0;i<nodes.size();i++){
        if (i == branchIndex) {
           nodes[i].position = QPoint((i+1-branch)*100+70,340);
           nodes[i+2].position = QPoint((i+2-branch)*100+70,260);
           nodes[i+1].position = QPoint((i+2-branch)*100+70,420);
           nodes[i+3].position = QPoint((i+3-branch)*100+70,340);
           branch++;
           i=i+3;
        }
        else{
           nodes[i].position = QPoint((i+1-branch)*100+70,340);
        }
    }
    nodes[0].currentlevel = true;

    // 发送生成的关卡信息
    emit levelGenerated(nodes, edges,branchIndex);
}
