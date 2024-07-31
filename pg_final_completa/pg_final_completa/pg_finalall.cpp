#include <iostream>
#include <string>
#include <cctype>
#include <random>
#include <chrono>
#include <fstream>
#include <windows.h>
using namespace std;
using namespace std::chrono;

class Player {
public:
    int leftHand;
    int rightHand;

    bool isOut() {
        return (leftHand == 0 && rightHand == 0);
    }

    void showHands() {
        cout << "��(L): " << leftHand << ", �E(R): " << rightHand << endl;
    }

    bool isHandAvailable(char hand) {
        hand = toupper(hand);
        if (hand == 'L') return leftHand > 0;
        else if (hand == 'R') return rightHand > 0;
        return false;
    }
};

void attack(Player& player1, Player& player2, char attackerHand, char defenderHand) {
    static int turn = 2;

    Player& attacker = (turn % 2 == 0) ? player1 : player2;
    Player& defender = (turn % 2 == 0) ? player2 : player1;

    attackerHand = toupper(attackerHand);
    defenderHand = toupper(defenderHand);

    if (attackerHand == 'L') {
        if (defenderHand == 'L') {
            defender.leftHand += attacker.leftHand;
        }
        else {
            defender.rightHand += attacker.leftHand;
        }
    }
    else {
        if (defenderHand == 'L') {
            defender.leftHand += attacker.rightHand;
        }
        else {
            defender.rightHand += attacker.rightHand;
        }
    }

    if (player2.leftHand == 5) player2.leftHand = 0;
    if (player2.leftHand > 5) player2.leftHand -= 5;
    if (player2.rightHand == 5) player2.rightHand = 0;
    if (player2.rightHand > 5) player2.rightHand -= 5;
    if (player1.leftHand == 5) player1.leftHand = 0;
    if (player1.leftHand > 5) player1.leftHand -= 5;
    if (player1.rightHand == 5) player1.rightHand = 0;
    if (player1.rightHand > 5) player1.rightHand -= 5;
    turn++;
}

void ranking_out(double time) {
    ofstream outfile("ranking.txt", ios_base::app);
    if (!outfile) {
        cout << "�t�@�C�����J�����Ƃ��ł��܂���ł����D�����L���O�͕\���ł��܂���D" << endl;
    }
    else {
        outfile << time << endl;
    }
}

void ranking_in(double time) {
    ifstream infile("ranking.txt");
    if (!infile) {
        cout << "�t�@�C����������܂���ł����D�����L���O�͕\���ł��܂���D" << endl;
    }
    else {
        string line;
        double t;
        vector<double> rank;
        while (getline(infile, line)) {
            t = stod(line);
            rank.emplace_back(t);
        }
        sort(rank.begin(), rank.end());
        cout << "�`�����L���O�`" << endl;
        for (int i = 0; i < 5; ++i) {
            if (i < rank.size()) {
                if (i > 0 && rank[i] == rank[i - 1]) {
                    cout << "   " << rank[i] << "�b" << endl;
                }
                else {
                    cout << i + 1 << ". " << rank[i] << "�b" << endl;
                }
            }
        }
        size_t r = rank.size();
        for (size_t i = 0; i < r; ++i) {
            if (time == rank[i]) {
                if (i + 1 < 6) {
                    cout << "�����N�C���I���߂łƂ��I�I" << endl; 
                    Beep(880, 100);
                    Beep(880, 500);
                    Beep(830, 450);
                    Beep(659, 100);
                    Beep(784, 400);
                }
                if (i + 1 < 16 && i + 1 > 5) {
                    cout << "�����N�C���܂ł��������I" << endl;
                }
                if (i + 1 < 51 && i + 1 > 15) {
                    cout << "�܂��܂����ڎw�����I" << endl;
                }
                if (i + 1 > 50) {
                    cout << "���������撣�낤..." << endl;
                    Beep(440, 150);
                    Beep(440, 500);
                }
                cout << "���Ȃ��̏��ʂ�" << i + 1 << "�ʂł��D" << endl;
                break;
            }
        }
    }
}

int main() {
    int turnlimit = 30;

    int mode;
    cout << "�v���C���[�h��I�����Ă��������D�@[1]�ΐl��C[2]�΃R���s���[�^��" << endl;
    cin >> mode;

    if (mode != 1 && mode != 2) {
        cout << "�L���ȃ��[�h(1or2)��I�����Ă��������D�Q�[�����I�����܂��D" << endl;
        return 0;
    }

    if (mode == 1) { //�ΐl��
        Player player1;
        player1.leftHand = 1;
        player1.rightHand = 1;
        Player player2;
        player2.leftHand = 1;
        player2.rightHand = 1;
        cout << "Game start!\n" << endl;
        Beep(800, 500);
        int turnSub = 1;

        while (!player1.isOut() && !player2.isOut()) {
            if (turnSub > turnlimit) {
                cout << "\nDraw! Game end!" << endl;
                break;
            }
            else {
                cout << "�Z�^�[��" << turnSub << "�FPlayer" << ((turnSub % 2 == 1) ? 1 : 2) << "�̃A�^�b�N" << endl;
                cout << "Player1...";
                player1.showHands();
                cout << "Player2...";
                player2.showHands();
                char attackerHand, defenderHand;

                do {
                    cout << "�U�����鎩���̎��I��ł�������(L/R): ";
                    cin >> attackerHand;
                    if (toupper(attackerHand) != 'L' && toupper(attackerHand) != 'R') {
                        cout << "L(��)��R(�E)�őI��ł��������B" << endl;
                    }
                    else if (!((turnSub % 2 == 1 ? player1 : player2).isHandAvailable(attackerHand))) {
                        cout << "���̎�͂����g���Ȃ��݂������c�����Е��̎��I��ł���I" << endl;
                    }
                } while ((toupper(attackerHand) != 'L' && toupper(attackerHand) != 'R') || !((turnSub % 2 == 1 ? player1 : player2).isHandAvailable(attackerHand)));

                do {
                    cout << "�U�����鑊��̎��I��ł�������(L/R): ";
                    cin >> defenderHand;
                    if (toupper(defenderHand) != 'L' && toupper(defenderHand) != 'R') {
                        cout << "L(��)��R(�E)�őI��ł��������B" << endl;
                    }
                    else if (!((turnSub % 2 == 1 ? player2 : player1).isHandAvailable(defenderHand))) {
                        cout << "���̎�͂����g���Ȃ��݂������c�����Е��̎��I��ł���I" << endl;
                    }
                } while ((toupper(defenderHand) != 'L' && toupper(defenderHand) != 'R') || !((turnSub % 2 == 1 ? player2 : player1).isHandAvailable(defenderHand)));

                attack(player1, player2, attackerHand, defenderHand);
                ++turnSub;
            }
        }
        if (turnSub <= turnlimit) {
            cout << "�Z�^�[��" << turnSub << "�FPlayer" << ((turnSub % 2 == 1) ? 1 : 2) << "�̃A�^�b�N" << endl;
            cout << "Player1...";
            player1.showHands();
            cout << "Player2...";
            player2.showHands();
            cout << "�����ƁC�N�̗���͂����g���Ȃ��݂������c" << endl;
            cout << "\nGame over!�@Player" << ((turnSub % 2 == 1) ? 2 : 1) << "�̏���!" << endl;
        }
    }

    else { //���[�h2
        random_device rnd;
        int turn = rnd() % 2;
        cout << "�΃R���s���[�^����n�߂܂��D";
        int turnSub = 1;
        string you = "You";
        string cpu = "CPU";
        auto t0 = high_resolution_clock::now();
        if (turn == 0) {
            cout << "���Ȃ��͐�U�ł��D" << endl;
            cout << "Game start!\n" << endl;
            Beep(800, 500);
            Player You;
            You.leftHand = 1;
            You.rightHand = 1;
            Player CPU;
            CPU.leftHand = 1;
            CPU.rightHand = 1;

            while (!You.isOut() && !CPU.isOut()) {
                if (turnSub > turnlimit) {
                    cout << "\nDraw! Game end!" << endl;
                    break;
                }
                else {
                    cout << "�Z�^�[��" << turnSub << "�F" << ((turnSub % 2 == 1) ? you : cpu) << "�̃A�^�b�N" << endl;
                    cout << "You...";
                    You.showHands();
                    cout << "CPU...";
                    CPU.showHands();
                    char attackerHand, defenderHand;
                    if (turnSub % 2 == 1) {
                        do {
                            cout << "�U�����鎩���̎��I��ł�������(L/R): ";
                            cin >> attackerHand;
                            if (toupper(attackerHand) != 'L' && toupper(attackerHand) != 'R') {
                                cout << "L(��)��R(�E)�őI��ł��������B" << endl;
                            }
                            else if (!(You.isHandAvailable(attackerHand))) {
                                cout << "���̎�͂����g���Ȃ��݂������c�����Е��̎��I��ł���I" << endl;
                            }
                        } while ((toupper(attackerHand) != 'L' && toupper(attackerHand) != 'R') || !(You.isHandAvailable(attackerHand)));

                        do {
                            cout << "�U�����鑊��̎��I��ł�������(L/R): ";
                            cin >> defenderHand;
                            if (toupper(defenderHand) != 'L' && toupper(defenderHand) != 'R') {
                                cout << "L(��)��R(�E)�őI��ł��������B" << endl;
                            }
                            else if (!(CPU.isHandAvailable(defenderHand))) {
                                cout << "���̎�͂����g���Ȃ��݂������c�����Е��̎��I��ł���I" << endl;
                            }
                        } while ((toupper(defenderHand) != 'L' && toupper(defenderHand) != 'R') || !(CPU.isHandAvailable(defenderHand)));
                        attack(You, CPU, attackerHand, defenderHand);
                    }
                    else {
                        int comHand = rnd() % 2;
                        int youHand = rnd() % 2;
                        if (comHand == 0) {
                            attackerHand = 'L';
                            if (!(CPU.isHandAvailable(attackerHand))) {
                                attackerHand = 'R';
                            }
                            if (youHand == 0) {
                                defenderHand = 'L';
                                if (!(You.isHandAvailable(defenderHand))) {
                                    defenderHand = 'R';
                                }
                            }
                            else {
                                defenderHand = 'R';
                                if (!(You.isHandAvailable(defenderHand))) {
                                    defenderHand = 'L';
                                }
                            }
                        }
                        else {
                            attackerHand = 'R';
                            if (!(CPU.isHandAvailable(attackerHand))) {
                                attackerHand = 'L';
                            }
                            if (youHand == 1) {
                                defenderHand = 'L';
                                if (!(You.isHandAvailable(defenderHand))) {
                                    defenderHand = 'R';
                                }
                            }
                            else {
                                defenderHand = 'R';
                                if (!(You.isHandAvailable(defenderHand))) {
                                    defenderHand = 'L';
                                }
                            }
                        }
                        cout << "CPU�̑I��...�U�����鎩���̎�F" << attackerHand << endl;
                        cout << "   �@�@�@   �U�����鑊��̎�G" << defenderHand << endl;
                        attack(You, CPU, attackerHand, defenderHand);
                    }
                }
                ++turnSub;
            }
            if (turnSub <= turnlimit) {
                cout << "�Z�^�[��" << turnSub << "�F" << ((turnSub % 2 == 1) ? you : cpu) << "�̃A�^�b�N" << endl;
                cout << "You...";
                You.showHands();
                cout << "CPU...";
                CPU.showHands();
                cout << "�����ƁC�N�̗���͂����g���Ȃ��݂������c" << endl;
                cout << "\nGame over!�@" << ((turnSub % 2 == 1) ? cpu : you) << "�̏���!" << endl;
                if (turnSub % 2 == 0) {
                    auto t1 = high_resolution_clock::now();
                    double time = duration_cast<milliseconds>(t1 - t0).count() / 1000.;
                    cout << "����̃^�C���F" << time << "�b\n";
                    ranking_out(time);
                    ranking_in(time);
                }
            }
        }

        else {
            cout << "���Ȃ��͌�U�ł��D" << endl;
            cout << "Game start!\n" << endl;
            Beep(800, 500);
            Player You;
            You.leftHand = 1;
            You.rightHand = 1;
            Player CPU;
            CPU.leftHand = 1;
            CPU.rightHand = 1;

            while (!You.isOut() && !CPU.isOut()) {
                if (turnSub > turnlimit) {
                    cout << "\nDraw! Game end!" << endl;
                    break;
                }
                else {
                    cout << "�Z�^�[��" << turnSub << "�F" << ((turnSub % 2 == 0) ? you : cpu) << "�̃A�^�b�N" << endl;
                    cout << "You...";
                    You.showHands();
                    cout << "CPU...";
                    CPU.showHands();
                    char attackerHand, defenderHand;
                    if (turnSub % 2 == 0) {
                        do {
                            cout << "�U�����鎩���̎��I��ł�������(L/R): ";
                            cin >> attackerHand;
                            if (toupper(attackerHand) != 'L' && toupper(attackerHand) != 'R') {
                                cout << "L(��)��R(�E)�őI��ł��������B" << endl;
                            }
                            else if (!(You.isHandAvailable(attackerHand))) {
                                cout << "���̎�͂����g���Ȃ��݂������c�����Е��̎��I��ł���I" << endl;
                            }
                        } while ((toupper(attackerHand) != 'L' && toupper(attackerHand) != 'R') || !(You.isHandAvailable(attackerHand)));

                        do {
                            cout << "�U�����鑊��̎��I��ł�������(L/R): ";
                            cin >> defenderHand;
                            if (toupper(defenderHand) != 'L' && toupper(defenderHand) != 'R') {
                                cout << "L(��)��R(�E)�őI��ł��������B" << endl;
                            }
                            else if (!(CPU.isHandAvailable(defenderHand))) {
                                cout << "���̎�͂����g���Ȃ��݂������c�����Е��̎��I��ł���I" << endl;
                            }
                        } while ((toupper(defenderHand) != 'L' && toupper(defenderHand) != 'R') || !(CPU.isHandAvailable(defenderHand)));
                        attack(CPU, You, attackerHand, defenderHand);
                    }
                    else {
                        int comHand = rnd() % 2;
                        int youHand = rnd() % 2;
                        if (comHand == 0) {
                            attackerHand = 'L';
                            if (!(CPU.isHandAvailable(attackerHand))) {
                                attackerHand = 'R';
                            }
                            if (youHand == 0) {
                                defenderHand = 'L';
                                if (!(You.isHandAvailable(defenderHand))) {
                                    defenderHand = 'R';
                                }
                            }
                            else {
                                defenderHand = 'R';
                                if (!(You.isHandAvailable(defenderHand))) {
                                    defenderHand = 'L';
                                }
                            }
                        }
                        else {
                            attackerHand = 'R';
                            if (!(CPU.isHandAvailable(attackerHand))) {
                                attackerHand = 'L';
                            }
                            if (youHand == 1) {
                                defenderHand = 'L';
                                if (!(You.isHandAvailable(defenderHand))) {
                                    defenderHand = 'R';
                                }
                            }
                            else {
                                defenderHand = 'R';
                                if (!(You.isHandAvailable(defenderHand))) {
                                    defenderHand = 'L';
                                }
                            }
                        }
                        cout << "CPU�̑I��...�U�����鎩���̎�F" << attackerHand << endl;
                        cout << "   �@�@�@   �U�����鑊��̎�G" << defenderHand << endl;
                        attack(CPU, You, attackerHand, defenderHand);
                    }
                }
                ++turnSub;
            }
            if (turnSub <= turnlimit) {
                cout << "�Z�^�[��" << turnSub << "�F" << ((turnSub % 2 == 0) ? you : cpu) << "�̃A�^�b�N" << endl;
                cout << "You...";
                You.showHands();
                cout << "CPU...";
                CPU.showHands();
                cout << "�����ƁC�N�̗���͂����g���Ȃ��݂������c" << endl;
                cout << "\nGame over!�@" << ((turnSub % 2 == 0) ? cpu : you) << "�̏���!" << endl;
                if (turnSub % 2 == 1) {
                    auto t1 = high_resolution_clock::now();
                    double time = duration_cast<milliseconds>(t1 - t0).count() / 1000.;
                    cout << "����̃^�C���F" << time << "�b\n";
                    ranking_out(time);
                    ranking_in(time);
                }
            }
        }
    }
}