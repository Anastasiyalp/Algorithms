#include <iostream>
#include <cstring>
#include <vector>

#include <windows.h>

using namespace std;
// Размер алфавита
const int k = 60;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct bohr_vrtx
{
    int next_vrtx[k];   //next_vrtx[i] — номер вершины, в которую мы придем по символу с номером i в алфавите
    int pat_num;        //номер строки-образца, обозначаемого этой вершиной
    int suff_link;      //suff_link - суффиксная ссылка
    int auto_move[k];   //auto_move - запоминание перехода автомата
    int par;            //par - вершина-отец в дереве
    int64_t suff_flink; //suff_flink - "хорошая" суф. ссылка
    bool flag;          //flag — бит, указывающий на то, является ли наша вершина исходной строкой
    char symb;          //символ на ребре от par к этой вершине
    int count_arc;      //количество исхдящих дуг из вершины
};

class Bohr
{
    vector<bohr_vrtx> bohr;
    vector<string> pattern;
    vector<pair<int, int>> remove_text;

    //Функция добваления вершины в бор
    bohr_vrtx make_bohr_vrtx(int p, char c) //передаем номер отца и символ на ребре в боре
    {
        bohr_vrtx v; //(255)=(2^8-1)=(все единицы в каждом байте памяти)=(-1 в дополнительном коде целого 4-байтного числа int)
        memset(v.next_vrtx, 255, sizeof(v.next_vrtx));
        memset(v.auto_move, 255, sizeof(v.auto_move));
        v.flag = false;   //По умолчанию: вершина - не сходная строка
        v.suff_link = -1; //изначально - суф. ссылки нет
        v.par = p;
        v.symb = c;
        v.suff_flink = -1;
        v.count_arc = 0;
        return v;
    }

  public:
    Bohr()
    {
        //добавляем единственную вершину - корень
        bohr.push_back(make_bohr_vrtx(0, '$'));
    }
    ~Bohr() = default;

    //Добавление строки в бор
    void add_string_to_bohr(const string &s)
    {
        int num = 0; //начинаем с корня
        for (int i = 0; i < s.length(); i++)
        {
            cout << '\t' << s[i] << " - ";
            int ch = s[i] - 'A';               //получаем номер в алфавите для символа строки
            if (bohr[num].next_vrtx[ch] == -1) //-1 - признак отсутствия ребра
            {
                cout << "символа в боре нет. Добавлим ребро для него." << endl;
                bohr[num].count_arc++;
                bohr.push_back(make_bohr_vrtx(num, ch));   //Создание вершины
                bohr[num].next_vrtx[ch] = bohr.size() - 1; //Установление ребра
            }
            else
                cout << "символ был в боре. Переходим к нему." << endl;
            num = bohr[num].next_vrtx[ch]; //Переход в следующую вершину
        }
        cout << "\tПометим конец строки." << endl;
        bohr[num].flag = true; //Пометка конца паттерна
        pattern.push_back(s);
        bohr[num].pat_num = pattern.size() - 1;
    }

  private:
    //Переход по суф. ссылке
    int get_suff_link(int v, int count_rec)
    {
        for(int i = 0; i < count_rec; i++)
            cout << '\t';
        cout <<"\tДля " << (v ? (char)(bohr[v].symb +'A') : '$') << " ";
        if (bohr[v].suff_link == -1)        //если еще не считали
        {
            cout << "суфф. ссылка не определена." << endl;
            for(int i = 0; i < count_rec; i++)
                cout << '\t';
            if (v == 0 || bohr[v].par == 0) //если v - корень или предок v - корень
            {
                cout << "\tМы у корня. Идти не куда. Следовательно суфф. ссылка ведет в корень." << endl;
                bohr[v].suff_link = 0;
            }else
            {
                cout << "\tСоздадим ее. Перейдем к родителю:" << endl;
                bohr[v].suff_link = get_auto_move(get_suff_link(bohr[v].par, count_rec + 1), bohr[v].symb);
            }
        }else
            cout << "суфф. ссылка уже была найдена. Она ведет к "<< (bohr[v].suff_link ? (char)(bohr[bohr[v].suff_link].symb +'A') : '$') << endl;
        return bohr[v].suff_link;
    }
    //Функция автомата
    int get_auto_move(int v, char ch)
    {
        if (bohr[v].auto_move[ch] == -1)                                     //Если для нашего случая переход автомата еще не определен, то определим его
        {
            cout << "\tАвтомат перехода не был определен. Определим его:" << endl;
            if (bohr[v].next_vrtx[ch] != -1)                                 //Если из вершины v есть путь в ch
            {
                cout << "\tБыла найдена дуга (" << (v ? (char)(bohr[v].symb +'A') : '$') <<", " << (char)(ch+'A') << ")" << endl;
                cout << "\tТеперь автомат определен." << endl;
                bohr[v].auto_move[ch] = bohr[v].next_vrtx[ch];               //Установим автомат перехода в нее
            }else if (v == 0)                                                 //Если пришли в корень
            {
                cout << "\tДуга (" << (v ? (char)(bohr[v].symb +'A') : '$') <<", " << (char)(ch+'A') << ") не найдена" << endl;
                cout << "\tПришли к корню. Теперь автомат определен." << endl;
                bohr[v].auto_move[ch] = 0;                                   //Установим автомат перехода в корень
            }else                                                             //Иначе
            {
                cout << "\tДуга (" << (v ? (char)(bohr[v].symb +'A') : '$') <<", " << (char)(ch+'A') << ") не найдена." << endl;
                cout << "\tПереходим по суффиксной ссылке." << endl;
                bohr[v].auto_move[ch] = get_auto_move(get_suff_link(v, 0), ch); //Установим автомат перехода рекурсивно в соответствии с автоматом для вершины в которую ведет суфф. ссылка текушей вершины
            }
        }else
            cout << "автомат перехода был определен.";
        return bohr[v].auto_move[ch];
    }
    // Нахождение "хорошей" суф. ссылки
    int get_suff_flink(int v, int count_rec)
    {
        if (bohr[v].suff_flink == -1)
        {
            int u = get_suff_link(v, count_rec); //Переход по суфф. ссылке
            if (u == 0)               //либо v - корень, либо суфф. ссылка v указывает на корень
                bohr[v].suff_flink = 0;
            else
                bohr[v].suff_flink = (bohr[u].flag) ? u : get_suff_flink(u, count_rec + 1); // В случае ненахождения конца паттерна, рекурсивный поиск
        }
        return bohr[v].suff_flink;
    }
    // Проверка подстроки на нахождение в боре
    void check(int v, int i)
    {
        for (int u = v; u != 0; u = get_suff_flink(u, 0)) //Переходы по суфф. ссылкам
        {
            if (bohr[u].flag)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << "Найдена подстрока: " << pattern[bohr[u].pat_num] << endl;
                cout << i - pattern[bohr[u].pat_num].length() + 1 << " " << bohr[u].pat_num + 1 << endl;
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED);
                remove_text.push_back({i - pattern[bohr[u].pat_num].length(), pattern[bohr[u].pat_num].size()}); //Добавление в вектор для последующей обработки текста
            }
        }
    }

  public:
    // Нахождение всех вхождений паттернов в текст
    void find_all_pos(const string &s)
    {
        int u = 0;
        for (int i = 0; i < s.length(); i++)
        {
            cout << endl << i+1<< ") "<< s[i] << " - ";
            u = get_auto_move(u, s[i] - 'A'); //Переход автомата
            cout << endl << "\tПроверка подстроки на нахождение в боре:" << endl;
            check(u, i + 1);                  // Проверка подстроки на нахождение в боре
            print_bohr(u);
        }
    }
    // Нахождение максиматльного кличества дуг в боре
    int max_arc()
    {
        int i_max_count_arc = 0;
        for (int i = 0; i < bohr.size(); i++)
            if (bohr[i_max_count_arc].count_arc < bohr[i].count_arc)
                i_max_count_arc = i;
        return bohr[i_max_count_arc].count_arc;
    }
    // Удаляет из текста все найденные паттерны
    string text_without_patterns(const string &s)
    {
        string final_text;
        bool remove_ind[s.length()]; //Массив пометок об удалении буквы из строки
        memset(remove_ind, true, s.length());
        for (int i = 0; i < remove_text.size(); i++) //Область паттерна помечена false
            memset(remove_ind + remove_text[i].first, false, remove_text[i].second);
        for (int i = 0; i < s.length(); i++)
            if (remove_ind[i]) //Для всех не помеченных букв: добавить к строке
                final_text += s[i];
        return final_text;
    }
    //Распечатка бора
    void print_bohr(int cur_vrtx)
    {
        if(cur_vrtx != -1)
            if(bohr[cur_vrtx].suff_link == 0)
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << bohr[0].symb << endl;
        if(cur_vrtx!= -1)
            if(bohr[cur_vrtx].suff_link == 0)
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED);
        print_next(0, 0, cur_vrtx);
    }

  private:
    void print_next(int n, int count_tab, int cur_vrtx) 
    {
        int next;
        int next_count_tab = 0;
        for (int i = 0; i < k; i++)
        {
            next = bohr[n].next_vrtx[i];
            if (next != -1)
            {
                cout << " - ";
                if(next == cur_vrtx)
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                if(cur_vrtx != -1)
                    if(bohr[cur_vrtx].suff_link == next)
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << (char)(bohr[next].symb + 'A');
                if(next == cur_vrtx)
                    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED);
                if(cur_vrtx!= -1)
                    if(bohr[cur_vrtx].suff_link == next)
                        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED);
                if (bohr[next].flag)
                    cout << "(" << bohr[next].pat_num + 1 << ")";
                print_next(next, count_tab + 1, cur_vrtx);
                next_count_tab++;
                if (bohr[next].count_arc == 0)
                    cout << endl;
                if (next_count_tab != bohr[n].count_arc)
                    for (int p = 0; p < count_tab; p++)
                        cout << "   ^";
            }
        }
    };

  public:
    //Распечатка паттернов
    void print_patterns()
    {
        for (int i = 0; i < pattern.size(); i++)
            cout << i + 1 << ") " << pattern[i] << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "rus");
    string T, pattern;
    int n;
    cout << "Введите текст и количество паттернов:" << endl;
    cin >> T >> n;
    // Инициализация бора
    cout << "Инициализация бора:" << endl;
    Bohr bohr;
    cout << "Добавление паттернов в бор:" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << i + 1 << ") ";
        cin >> pattern;
        // Добавление паттернов в бор
        bohr.add_string_to_bohr(pattern);
        bohr.print_bohr(-1);
    }
    cout << "Итоговое предсталение бора для списка паттернов:" << endl;
    bohr.print_patterns();
    bohr.print_bohr(-1);
    cout << endl << endl << "Нахождение всех паттернов в строке " << T << ":" << endl;
    bohr.find_all_pos(T);
    cout << "max_arc : " << bohr.max_arc() << endl;
    cout << "Text without patterns: " << bohr.text_without_patterns(T) << endl;
}