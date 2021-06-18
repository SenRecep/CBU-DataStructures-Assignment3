#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <cstdio>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;

#pragma region Enums
enum OrderMode
{
    PreOrder,
    InOrder,
    PostOrder
};
enum Option
{
    Search,
    List,
    Delete,
    WriteFile,
    Exit
};
enum SearchType
{
    DFS,
    BFS
};
enum TreeType
{
    Data1,
    Data2,
    Data3
};
#pragma endregion

#pragma region Student
class Student
{
public:
    int No;
    string FirstName;
    string LastName;
    Student(int no = -1, string firstName = "", string lastname = "") : No(no), FirstName(firstName), LastName(lastname) {}
    bool operator<(const Student &s) const
    {
        return No < s.No;
    }
    bool operator==(const Student &s) const
    {
        return No == s.No;
    }
    friend ostream &operator<<(ostream &out, Student &s)
    {
        out << s.No << " " << s.FirstName << " " << s.LastName;
        return out;
    }
    static Student Create(string str)
    {
        Student student;
        string temp;
        int start = 0, end = str.find("\t"), counter = 0;
        while (end != -1)
        {
            string data = str.substr(start, end - start);
            if (++counter == 1)
                student.No = stoi(data);
            else if (counter == 2)
                student.FirstName = data;
            else if (counter == 3)
                student.LastName = data;
            start = end + 1;
            end = str.find(" ", start);
            if (end == -1 || counter > 3)
            {
                student.FirstName += " " + student.LastName;
                student.LastName = str.substr(start, end - start);
            }
        }
        return student;
    }
};
#pragma endregion

#pragma region Nodes
template <class T>
class Node
{
public:
    T Data;
    Node<T> *Left;
    Node<T> *Right;
    Node(T data = T(), Node<T> *left = NULL, Node<T> *right = NULL)
        : Data(data), Left(left), Right(right) {}
};
#pragma endregion

#pragma region Data Structures
template <class T>
class Tree
{
private:
    Node<T> *root;
    Node<T> *insert(Node<T> *start, T data)
    {
        if (start)
        {
            if (data < start->Data)
                start->Left = insert(start->Left, data);
            else
                start->Right = insert(start->Right, data);
        }
        else
            return new Node<T>(data);
        return start;
    }
    Node<T> *find(Node<T> *start, T data)
    {
        if (start)
        {
            if (data < start->Data)
                return find(start->Left, data);
            else if (start->Data < data)
                return find(start->Right, data);
        }
        return start;
    }
    Node<T> *getMin(Node<T> *start)
    {
        while (start && start->Left)
            start = start->Left;
        return start;
    }
    Node<T> *remove(Node<T> *start, Node<T> *deleted)
    {
        if (start)
        {
            if (deleted->Data < start->Data)
                start->Left = remove(start->Left, deleted);
            else if (start->Data < deleted->Data)
                start->Right = remove(start->Right, deleted);
            else if (start == deleted)
            {
                Node<T> *temp = NULL;
                if (start->Left && start->Right)
                {
                    temp = getMin(start->Right);
                    remove(start, temp);
                    temp->Left = start->Left;
                    temp->Right = start->Right;
                }
                else if (start->Left)
                    temp = start->Left;

                else if (start->Right)
                    temp = start->Right;
                return temp;
            }
            else
                start->Right = remove(start->Right, deleted);
        }
        return start;
    }
    void preOrder(Node<T> *start, ostream &out)
    {
        if (start)
        {
            out << start->Data << endl;
            preOrder(start->Left, out);
            preOrder(start->Right, out);
        }
    }
    void inOrder(Node<T> *start, ostream &out)
    {
        if (start)
        {
            inOrder(start->Left, out);
            out << start->Data  << endl;
            inOrder(start->Right, out);
        }
    }
    void postOrder(Node<T> *start, ostream &out)
    {
        if (start)
        {
            postOrder(start->Left, out);
            postOrder(start->Right, out);
            out << start->Data  << endl;
        }
    }

public:
    Tree(Node<T> *root = NULL) : root(root) {}
    bool IsEmpty() const { return root == NULL; }
    Node<T> *GetRoot() { return root; }
    Node<T> *Find(T data, bool last = false)
    {
        Node<T> *before = find(root, data);
        Node<T> *after = before;
        if (last)
            while (before && (before = find(before->Right, data)))
                after = before;
        return after;
    }
    Node<T> *Insert(T data)
    {
        root = insert(root, data);
    }
    Node<T> *GetMin()
    {
        return getMin(root);
    }
    void RemoveNode(Node<T> *node)
    {
        if (node)
        {
            root = remove(root, node);
            delete node;
        }
    }
    void Remove(T data, bool last = false)
    {
        RemoveNode(Find(data, last));
    }
    void Print(OrderMode mode)
    {
        switch (mode)
        {
        case PreOrder:
            preOrder(root, cout);
            break;
        case InOrder:
            inOrder(root, cout);
            break;
        case PostOrder:
            postOrder(root, cout);
            break;
        }
        if (!IsEmpty())
            cout << endl;
    }
    void Write(OrderMode mode, TreeType type)
    {
        string path = "../data/output_";
        switch (type)
        {
        case Data1:
            path += "data1.txt";
            break;
        case Data2:
            path += "data2.txt";
            break;
        case Data3:
            path += "data3.txt";
            break;
        }
        ofstream file;
        file.open(path);
        if (file.is_open())
        {
            switch (mode)
            {
            case PreOrder:
                preOrder(root, file);
                break;
            case InOrder:
                inOrder(root, file);
                break;
            case PostOrder:
                postOrder(root, file);
                break;
            }
            file.close();
        }
    }
    T DepthFirstSearch(T data)
    {
        Node<T> *temp;
        stack<Node<T> *> stack;
        stack.push(root);
        while (!stack.empty())
        {
            temp = stack.top();
            stack.pop();
            if (data == temp->Data)
                return temp->Data;
            if (temp->Left)
                stack.push(temp->Left);
            if (temp->Right)
                stack.push(temp->Right);
        }
        return T();
    }
    T BreadthFirstSearch(T data)
    {
        Node<T> *temp;
        queue<Node<T> *> queue;
        queue.push(root);
        while (!queue.empty())
        {
            temp = queue.front();
            queue.pop();
            if (data == temp->Data)
                return temp->Data;
            if (temp->Left)
                queue.push(temp->Left);
            if (temp->Right)
                queue.push(temp->Right);
        }
        return T();
    }
};

#pragma endregion

#pragma region Helpers
class TreeState
{
public:
    TreeState(Tree<Student> *tree1 = NULL, Tree<Student> *tree2 = NULL, Tree<Student> *tree3 = NULL)
        : Data1Tree(tree1), Data2Tree(tree2), Data3Tree(tree3) {}
    Tree<Student> *Data1Tree;
    Tree<Student> *Data2Tree;
    Tree<Student> *Data3Tree;
};

class TreeHelper
{
public:
    static void Init(Tree<Student> *tree, string fileName)
    {
        ifstream file(fileName);
        if (file.is_open())
        {
            while (!file.eof())
            {
                string line;
                getline(file, line);
                Student student = Student::Create(line);
                if (student.No != -1)
                    tree->Insert(student);
            }
            file.close();
        }
    }
    static Tree<Student> *Init(string fileName)
    {
        Tree<Student> *tree = new Tree<Student>();
        TreeHelper::Init(tree, fileName);
        return tree;
    }
    static Tree<Student> *GetTree(TreeType type, TreeState *state)
    {
        switch (type)
        {
        case Data1:
            return state->Data1Tree;
        case Data2:
            return state->Data2Tree;
        case Data3:
            return state->Data3Tree;
        }
    }
};

class MenuHelper
{
private:
    string selection;
    TreeState *state;

public:
    MenuHelper(TreeState *state = NULL) : state(state){};
    void Hr()
    {
        cout << "\n___________________\n\n";
    }

    OrderMode OrderSelection()
    {
        if (selection == "1")
            return PreOrder;
        else if (selection == "2")
            return InOrder;
        else
            return PostOrder;
    }
    TreeType TreeSelection()
    {
        if (selection == "1")
            return Data1;
        else if (selection == "2")
            return Data2;
        else
            return Data3;
    }
    SearchType SearchSelection()
    {
        if (selection == "1")
            return DFS;
        else
            return BFS;
    }
    Option MenuSelection()
    {
        if (selection == "A" || selection == "a")
            return Search;
        else if (selection == "L" || selection == "l")
            return List;
        else if (selection == "S" || selection == "s")
            return Delete;
        else if (selection == "Y" || selection == "y")
            return WriteFile;
        else
            return Exit;
    }

    void DeleteMenu()
    {
        int no;
        cout << "Silmek istediginiz ogrencinin numarasini giriniz: ";
        cin >> no;
        Hr();
        TreeType treeType = TreeMenu();
        Tree<Student> *tree = TreeHelper::GetTree(treeType, state);
        tree->Remove(Student(no));
        cout << "Istlem tamamlandi" << endl;
    }

    void WriteFileMenu()
    {
        TreeType treeType = TreeMenu();
        Tree<Student> *tree = TreeHelper::GetTree(treeType, state);
        cout << "Hangi yazdirma yontemini istiyorsunuz" << endl
             << "Preorder: 1" << endl
             << "Inorder: 2" << endl
             << "Postorder: 3" << endl
             << "Secim yapiniz: ";
        cin >> selection;
        Hr();
        OrderMode orderMode = OrderSelection();
        tree->Write(orderMode, treeType);
    }

    void ListMenu()
    {
        cout << "Hangi listeleme yontemini istiyorsunuz" << endl
             << "Preorder: 1" << endl
             << "Inorder: 2" << endl
             << "Postorder: 3" << endl
             << "Secim yapiniz: ";
        cin >> selection;
        Hr();
        OrderMode orderMode = OrderSelection();
        TreeType treeType = TreeMenu();
        Tree<Student> *tree = TreeHelper::GetTree(treeType, state);
        tree->Print(orderMode);
    }

    TreeType TreeMenu()
    {
        cout << "Hangi agac uzerinde islem yapmak istiyorsunuz" << endl
             << "data1.txt: 1" << endl
             << "data2.txt: 2" << endl
             << "data3.txt: 3" << endl
             << "Secim yapiniz: ";
        cin >> selection;
        Hr();
        return TreeSelection();
    }

    void SearchMenu()
    {
        int no;
        cout << "Depth First Search: 1" << endl
             << "Breath First Search: 2" << endl
             << "Secim yapiniz: ";
        cin >> selection;
        Hr();
        SearchType searchType = SearchSelection();
        cout << "Aramak istediginiz ogrencinin numarasini giriniz: ";
        cin >> no;
        Hr();
        TreeType treeType = TreeMenu();
        Tree<Student> *tree = TreeHelper::GetTree(treeType, state);
        Student found;
        switch (searchType)
        {
        case DFS:
            found = tree->DepthFirstSearch(no);
            break;
        case BFS:
            found = tree->BreadthFirstSearch(no);
            break;
        }
        if (found.No != -1)
            cout << found << endl;
        else
            cout << "Ilgili ogrenci bulunamadi" << endl;
    }

    void MainMenu()
    {
        while (true)
        {
            system("cls");
            cout << "Arama Yap: A" << endl
                 << "Liste: L" << endl
                 << "Ogrenci Sil: S" << endl
                 << "Dosyaya Yaz: Y" << endl
                 << "Cikis: C" << endl
                 << "Secim yapiniz: ";
            cin >> selection;
            Option option = MenuSelection();
            Hr();
            switch (option)
            {
            case Search:
                SearchMenu();
                break;
            case List:
                ListMenu();
                break;
            case Delete:
                DeleteMenu();
                break;
            case WriteFile:
                WriteFileMenu();
                break;
            default:
                return;
            }
            system("PAUSE");
        }
    }
};
#pragma endregion

class Test
{
private:
    int testNumberCount = 11;
    int studentNumbers[11] = {1001067,1001072,1001075,1001083,1003002,1003009,1003021,1003028,1003033,1003038,1003041};
    TreeState *state;
    double beginDepthFirstSearch(Tree<Student> *tree)
    {
        double total = 0;
        for (int no : studentNumbers)
        {
            auto start = high_resolution_clock::now();
            tree->DepthFirstSearch(no);
            auto stop = high_resolution_clock::now();
            microseconds duration = duration_cast<microseconds>(stop - start);
            total += duration.count();
        }
        return total / testNumberCount;
    }
    double beginBreadthFirstSearch(Tree<Student> *tree)
    {
        double total = 0;
        for (int no : studentNumbers)
        {
            auto start = high_resolution_clock::now();
            tree->BreadthFirstSearch(no);
            auto stop = high_resolution_clock::now();
            microseconds duration = duration_cast<microseconds>(stop - start);
            total += duration.count();
        }
        return total / testNumberCount;
    }

public:
    Test(TreeState *state) : state(state) {}
    void Begin()
    {
        Tree<Student> *tree = state->Data1Tree;
        double d1_avgDFS = beginDepthFirstSearch(tree);
        double d1_avgBFS = beginBreadthFirstSearch(tree);
        cout << "data1.txt icin " << testNumberCount << " Ogrencinin ortalama bulunma sureleri:" << endl;
        cout << "DepthFirstSearch: " << d1_avgDFS << " BreadthFirstSearch: " << d1_avgBFS << endl;
        tree = state->Data2Tree;
        double d2_avgDFS = beginDepthFirstSearch(tree);
        double d2_avgBFS = beginBreadthFirstSearch(tree);
        cout << "data2.txt icin " << testNumberCount << " Ogrencinin ortalama bulunma sureleri:" << endl;
        cout << "DepthFirstSearch: " << d2_avgDFS << " BreadthFirstSearch: " << d2_avgBFS << endl;
        tree = state->Data3Tree;
        double d3_avgDFS = beginDepthFirstSearch(tree);
        double d3_avgBFS = beginBreadthFirstSearch(tree);
        cout << "data3.txt icin " << testNumberCount << " Ogrencinin ortalama bulunma sureleri:" << endl;
        cout << "DepthFirstSearch: " << d3_avgDFS << " BreadthFirstSearch: " << d3_avgBFS << endl;
    }
};

int main()
{
    TreeState *state = new TreeState(
        TreeHelper::Init("../data/data1.txt"),
        TreeHelper::Init("../data/data2.txt"),
        TreeHelper::Init("../data/data3.txt"));
    MenuHelper MenuHelper(state);
    MenuHelper.MainMenu();
    Test *test = new Test(state);
    test->Begin();
    delete state, test;
    return 0;
}