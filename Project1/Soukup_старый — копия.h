#include <fstream>
#include <math.h>
#include <stack> 
#include <queue>
#include <vector>
#include <ctime>


using namespace std;
int size_k = 0;
int n = 1, count_barrier, bx, by, count_trace, ist_x, ist_y, target_x, target_Y;
bool Lblock[10000000], Sblock[10000000];
int Lpath[10000000], Spath[10000000],
dx[4] = { -1,0,1,0 }, dy[4] = { 0,-1,0,1 };
bool visit[10000000];
int newDRP[10000][10000];
vector<int> startX_mass;
vector<int> startY_mass;
vector<int> target_X_mass;
vector<int> target_Y_mass;

int super_id;

//int newDRP[100][100];
char Lboard[10000][10000], Sboard[10000][10000];

void print_termenal_and_barrier();
void print_DRP();
void Print_newDRP(char Sboard[10000][10000]);


int TO_TARGET(int curr, int target)                                                             //���������� �� ���������
{
    int Manh_dist;
    int tx = target / n, ty = target % n, cx = curr / n, cy = curr % n;                         //����� ���������� ���������� � � � ��������� � ���������
    Manh_dist = abs(tx - cx) + abs(ty - cy);
    return Manh_dist;                                                                             //������� ��������������� ����������
}

int DIRECTION(int next, bool visit[], int target)                                                    //�����������
{
    int Manh_dist = INT_MAX;
    int tx = target / n, ty = target % n, nx = next / n, ny = next % n;                              //����� ���������� ���������� � � � ��������� � ���������

    for (int i = 0; i < 4; i++)
    {
        int x = nx + dx[i], y = ny + dy[i];                                                             //��
        int id = x * n + y;
        if ((x >= 0 && x < n && y >= 0 && y < n) && visit[id] == false && Sblock[id] == false)          //���������� ���������� ���������� ����� ���������� � ���������� � ����������� �� ������ ����������� ???
        {
            if (Manh_dist > abs(tx - x) + abs(ty - y))
            {
                Manh_dist = abs(tx - x) + abs(ty - y);
                super_id = id;
            }
        }
    }
    return Manh_dist;                                                                                         //������� �����������, ������� � �������� ������� ����������� ���������� ??
}

int NGHBR_IN_DIR(int curr, bool visit[], int target_func)                                   //�������� �� ����� �����������????
{
    int x = curr / n, y = curr % n;                                                 //
    bool flag = true;
    int id;

    if (DIRECTION(curr, visit, target_func) <= TO_TARGET(curr, target_func))
    {
        flag = false;
        id = super_id;
    }
    return flag == true ? -1 : id;
}

bool SoukupAlgo(int x_1, int y_1, int x_2, int y_2)
{
    
    memset(visit, false, (n * n) + 1);                              //��������� ��� 0

    int start_func = x_1 * n + y_1, target_func = x_2 * n + y_2;                       //

    stack<int>FILO_stack;                                                //FILO
    queue<int>LIFO_queue;                                                //LIFO   (������������ �������)           

    FILO_stack.push(start_func);                                                //��������� ������� � �������
    Spath[start_func] = -1;
    visit[start_func] = true;

    while (!FILO_stack.empty())                             //���� � ������� ���� ��-��    
    {
        int first_element = FILO_stack.top();              //������ ������� � �������

        if (first_element == target_func)
        {
            return true;
        }

        if (DIRECTION(first_element, visit, target_func) <= TO_TARGET(first_element, target_func))      //���������� ���������� �� ������� ��������� �� ��� ��������� � ���������� ��
        {
            int id = super_id;

            FILO_stack.push(id);                             //��������� ������� � �������
            visit[id] = true;
            Spath[id] = first_element;
            if (id == target_func)
            {
                return true;
            }

            while (NGHBR_IN_DIR(id, visit, target_func) >= 0)               //���� ����������� �� ��������, �������� �� ���� �� ���� �� ��������� ���������
            {
                int new_id = NGHBR_IN_DIR(id, visit, target_func);
                //cout<<"in FILO_stack "<<new_id<<endl;
                FILO_stack.push(new_id);                                     //��������� ������� � �������
                visit[new_id] = true;
                Spath[new_id] = id;
                if (new_id == target_func)
                {
                    return true;
                }
                id = new_id;
            }
        }
        while (!FILO_stack.empty())                                          //���� ���� �������� � �������
        {
            first_element = FILO_stack.top();                                          //���������� � ������� �������� � �����
            int tx = first_element / n, ty = first_element % n;                             //���������� ����-�� ����� � ������ ������ 
            for (int i = 0; i < 4; i++)
            {
                int x = tx + dx[i], y = ty + dy[i];
                int id = x * n + y;
                if ((x >= 0 && x < n && y >= 0 && y < n) && visit[id] == false && Sblock[id] == false)                  //���������� ������� ���������� �����������
                {
                    //cout<<"in LIFO_queue "<<id<<endl;
                    LIFO_queue.push(id);                                     //�������� ������� � �������
                    visit[id] = true;
                    Spath[id] = first_element;
                }
            }
            FILO_stack.pop();                                        //������� ������ ������� �� �������
        }
        while (!LIFO_queue.empty())                                  //���� � ������� ���� ��������                              
        {
            FILO_stack.push(LIFO_queue.front());                          //��������� � FILO_stack ������ ������� �� LIFO_queue
            LIFO_queue.pop();                                        //������� ������ ������� �� �������
        }
    }
    return false;
}

bool input() {
    for (auto component : def.components) {								//read component in DEF
        int x_1 = component->x_position;								//position vertex x1,y1
        int y_1 = component->y_position;

        std::string  name_in_Macro = component->name_model_in_LEF;
        std::string name_wire;
        for (auto elements : ver.elements) {                             //open verilog elements
            if (elements->name_component_in_def == component->name)         //name verilog element = name component in def
            {
                double mass_koord[4];
                //coord tempi;
                for (auto pins : elements->pins) {                           //open pins
                    if (pins->using_flag == true)  continue;                    //checking for repetition
                    name_wire = pins->name_wire;
                    std::string name_pin = pins->name_pin;

                    //find first position

                    for (auto macro : lef.macroes) {                         //open macro
                        if (macro->name == name_in_Macro)
                        {
                            for (auto pin_macro : macro->pins) {            //open pin in macro
                                if (pin_macro->name == name_pin)
                                {
                                    for (auto polygon : pin_macro->polygons) {				//open polygon
                                        int i = 0;
                                        for (auto position : polygon->position) {
                                            mass_koord[i] = position;					//read position
                                            ++i;
                                        }
                                        break;                                          //close polygon
                                    }
                                    break;                                      //close pin in macro
                                }
                            }
                            break;                                                      //close macro
                        }
                    }
                    //name_wire = pins->name_wire;      
                    //save name_wire
                    startX_mass.push_back (x_1 + (mass_koord[0] + mass_koord[2]) / 2);                      //x_start
                    startY_mass.push_back (y_1 + (mass_koord[1] + mass_koord[3]) / 2);                      //y_start
                    //source.z = 1;                                                            //z_start
                    //start.push_back(tempi);                                             //push_back koor-d
                    pins->using_flag = true;                                            //marker pin



                    //find second position
                    int second_x_1 = 0;								//position vertex x1,y1
                    int second_y_1 = 0;

                    for (auto second_element : ver.elements) {                          //open verilog elements

                        double mass_koord[4];
                        //coord tempj;
                        for (auto second_pins : second_element->pins) {                   //open pins
                            if (second_pins->using_flag == true) continue;               //checking for repetition
                            if (second_pins->name_wire == name_wire) {                  //
                                name_pin == second_pins->name_pin;                       //save name pin in verilog


                                std::string name_DEF_element = second_element->name_component_in_def;     //save DEF name element in verilog
                                std::string name_LEF_element = second_element->name;                     //save LEF name element in verilog

                                for (auto DEF_element : def.components) {                               //open DEF components
                                    if (DEF_element->name == name_DEF_element) {
                                        second_x_1 = DEF_element->x_position;                          //veftex  second x1, second y1;
                                        second_y_1 = DEF_element->y_position;
                                        break;                                                  //close for auto
                                    }
                                }
                                for (auto LEF_macro : lef.macroes) {                                    //open LEF macro
                                    for (auto MacroPin : LEF_macro->pins) {                             //open pins in macro
                                        if (MacroPin->name == name_pin) {

                                            for (auto second_polygon : MacroPin->polygons) {				//open polygon
                                                int i = 0;
                                                for (auto position : second_polygon->position) {
                                                    mass_koord[i] = position;					//read position
                                                    ++i;
                                                }
                                                break;                                          //close polygon
                                            }
                                            break;                                      //close pin in macro
                                        }
                                        else continue;
                                    }
                                    break;
                                }
                                //int i = 0;
                                target_X_mass.push_back (second_x_1 + (mass_koord[0] + mass_koord[2]) / 2);                     //x_start
                                target_Y_mass.push_back (second_y_1 + (mass_koord[1] + mass_koord[3]) / 2);                     //y_start
                                //target.z = 1;                                                            //z_start
                               // enda.push_back(tempj);                                             //push_back koor-d
                                second_pins->using_flag = true;                                     //marker pin
                                //classicalImplementation(l1, l2, l3, x, y, source, target, via, swapCoord, floodLessB);
                                //route--;
                                break;
                            }
                        }
                        continue;
                    }
                    continue;
                }

            }
            continue;
        }
    }
    return 0;
}

bool Soukup()
{
    setlocale(LC_ALL, "Russian");

    //cout << endl << "�� ��������� ���������, ����������� �������� ������� ����������� ������� �������." << endl
    //    << "������ �������� ������� ������ ��-44 ���������� �����." << endl
    //    << "�������� ������� �������� �������������� ����������. ��� ������������ ����� ���� �������� 10�10 ����� � �������������" << endl
    //    << "������� ���������� ���������� S1, S2, S3 � T1, T2, T3 ��������������, � ����� input " << endl;

    //ifstream in("input.txt");                                                                       //��������� ������ ������ 
    ofstream out2("soukup.txt");

    input();
    //if (!input()) {
    //    std::cout << std::endl << std::endl << "_err_ : Trace error!" << std::endl;
    //    return EXIT_FAILURE;
    //}
    std::cout << std::endl << std::endl << " Datas reading" << std::endl;

    n = startX_mass.size();
    memset(Lblock, false, (n * n) + 1);                                                                 //��������� ����� ������ Lblock � Sblock 0
    memset(Sblock, false, (n * n) + 1);

   // for (int i = 0; i < count_barrier; i++)                                                               //����������� ����������� �� �����
   // {
   //     in >> bx >> by;
   //     Lboard[bx][by] = '#';
   //     Sboard[bx][by] = '#';
   //     Lblock[bx * n + by] = true;                                                                         //��
   //     Sblock[bx * n + by] = true;
   // }
    bool ans[2][10000];

   // in >> count_trace;                                                                                  //���-�� ����� (�����) 

   // cout << endl << endl;
   ////_getch();
   // print_termenal_and_barrier();
   // cout << endl << endl;

    /* ������� � ��������� 4 �������
     1) ������ � ����-� ����������
     2) ������ � ����-� ����������
     3) ������ � ����-� ����������
     4) ������ � ����-� ����������
     ���������� ��������� � ��������� ���������������� ���������
     ������� � �������*/

    
    //for (int i = 0; i < count_trace; i++)                                                                   //����������� ��������� �� �����
    //{
    //    in >> ist_x >> ist_y >> target_x >> target_Y;                                     //��������� ����-�� ��������� � ����-�� ���������
    //    startX_mass[i] = ist_x;                                       //start_x
    //    startY_mass[i] = ist_y;                                       //start_y
    //    target_X_mass[i] = target_x;                                     //target_x
    //    target_Y_mass[i] = target_Y;                                     //target_y
    //    Lboard[ist_x][ist_y] = 'S';
    //    Lboard[target_x][target_Y] = 'T';
    //    Sboard[ist_x][ist_y] = 'S';
    //    Sboard[target_x][target_Y] = 'T';
    //}
    //print_termenal_and_barrier();
    //cout << endl << endl;
    //Timer t;

    count_trace = startX_mass.size();
    //--------------------------for Soukup Algorithm---------------------
    out2 << "Soukup Algorithm : " << endl;

    for (int i = 0; i < count_trace; i++)                                       //��� ������ ���� ��������-��������
    {
        ist_x = startX_mass[i];
        ist_y = startY_mass[i];
        target_x = target_X_mass[i];
        target_Y = target_Y_mass[i];
        //for Lee's algorithm
        if (SoukupAlgo(ist_x, ist_y, target_x, target_Y) == true)                           //���� ����������� ������ �������, ������������� �� � �������
        {

            int id = target_x * n + target_Y;
            Sblock[id] = true;
            Sblock[ist_x * n + ist_y] = true;
            while (Spath[id] != -1)
            {
                Sboard[Spath[id] / n][Spath[id] % n] = '*';
                Sblock[Spath[id]] = true;
                id = Spath[id];
            }
            Sboard[ist_x][ist_y] = 'S';
            ans[1][i] = true;
            cout << endl << "Trace " << i << " ready";
        }
        
        //else                                                         //���� ������ �������� �����������
        //{
        //    Sboard[ist_x][ist_y] = 's';
        //    Sboard[target_x][target_Y] = 't';
        //    ans[1][i] = false;
        //}
    }
    //double time_algotim = t.elapsed();
    cout << endl << "Trace completed successfully";
    for (int i = 0; i < n; i++)                                                     //����� ���������� � ����
    {
        for (int j = 0; j < n; j++) out2 << Sboard[i][j] << " ";
        out2 << endl;
    }
    out2 << endl;
    for (int i = 0; i < count_trace; i++)                                               //���� ������ �������� �����������
    {
        if (ans[1][i] == false)
        {
            out2 << " (" << ist_x << "," << ist_y << ")->(" << target_x << "," << target_Y << ") ���������� �������� �����������." << endl;
        }
    }

    return 0;
}