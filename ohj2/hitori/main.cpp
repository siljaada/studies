#include <iostream>
#include <vector>
#include <random>

using namespace std;

const unsigned int BOARD_SIDE = 5;
const unsigned char EMPTY = ' ';
const unsigned int NOT_EXSIST = 10;

// Muuttaa annetun numeerisen merkkijonon vastaavaksi kokonaisluvuksi
// (kutsumalla stoi-funktiota).
// Jos annettu merkkijono ei ole numeerinen, palauttaa nollan.
//

unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Tulostaa pelilaudan rivi- ja sarakenumeroineen.
//

void print(const vector<vector<int>>& gameboard)
{
    cout << "=================" << endl;
    cout << "|   | 1 2 3 4 5 |" << endl;
    cout << "-----------------" << endl;
    for(unsigned int i = 0; i < BOARD_SIDE; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < BOARD_SIDE; ++j)
        {
            if(gameboard.at(i).at(j) == 0)
            {
                cout << EMPTY << " ";
            }
            else
            {
                cout << gameboard.at(i).at(j) << " ";
            }
        }
        cout << "|" << endl;
    }
    cout << "=================" << endl;
}

// Luo pelilaudan käyttäjän valitsemalla tavalla joko arvotuilla luvuilla tai syötetyillä
// luvuilla.

void make_board(vector<vector<int>> &board, vector<int> &row)
{
    string input;

    // Kysyy käyttäjältä pelilaudan rakennustyyliä niin kauan, kunnes halutun tyylin
    // kirjain syötetään.

    while (input != "R" and input != "I" and input != "i" and input != "r")
    {
        cout << "Select start (R for random, I for input): ";
        getline(cin, input);
    }
    //Luo arvotuilla luvuilla pelilaudan, joka on muotoa vektori, jonka sisässä vektori,
    // jossa numeroita.
    if (input == "R" or input =="r")
    {
        string seed;
        cout << "Enter seed value: ";
        cin >> seed;
        default_random_engine rand_gen;
        rand_gen.seed( stoi_with_check(seed) );
        uniform_int_distribution<int> distribution(1, 5);
        for( unsigned int y = 0; y < BOARD_SIDE; ++y )
        {
            for( unsigned int x = 0; x < BOARD_SIDE; ++x )
            {
                row.push_back( distribution(rand_gen) );
            }
            board.push_back( row );
            row.clear();
        }
        print(board);
    }
    // Luo pelilaudan annetuilla luvuilla.
    if (input == "I" or input == "i" )
    {
        string numbers;
        cout << "Input: ";
        for (unsigned int i = 0; i < BOARD_SIDE; ++i ){

            for (unsigned int a =0; a < BOARD_SIDE; a++) {
              cin >> numbers;
              row.push_back(stoi_with_check(numbers));
            }
            board.push_back(row);
            row.clear();
            }

        print(board);

        }
}

// Tarkastelee onko peli hävitty, eli löytyykö numero, jolla ei ole vierekkäisiä numeroita
// tai kahta allekkaista tai vierekkäistä tyhjää. Paluttaa true, jos on hävitty.
// Ensiksi luodaan muuttujat jokaiselle suunnalle, mutta reunapaloissa ei kaikkia suuntia
// joten tutkitaan, että jos on reunassa, muuttujasta tulee NOT_EXSIST.

bool is_loosing(vector<vector<int>> board)
{
  unsigned int up;
  unsigned int down;
  unsigned int right;
  unsigned int left;

  // Annetaan muuttujille arvot riippuen niiden sijainnista, joka tarkasteltu
  // if/else rakenteella

  for (unsigned int y=0; y < BOARD_SIDE; ++y)
  {
      for (unsigned int x =0; x < BOARD_SIDE; ++x )
      {
          if (x ==0)
          {
              left = NOT_EXSIST;
          }
          else
          {
              left = board.at(y).at(x-1);
          }
          if (y == 0)
          {
              up = NOT_EXSIST;
          }
          else
          {
              up = board.at(y-1).at(x);
          }
          if (x == 4)
          {
              right = NOT_EXSIST;
          }
          else
          {
              right = board.at(y).at(x+1);
          }
          if (y==4)
          {
              down = NOT_EXSIST;
          }
          else
          {
              down = board.at(y+1).at(x);
          }

          // lopuksi tehdään häviön tarkastelu, jos ruutu on tyhjä onko vieressä tyhjää
          // Jos ruutu ei ole tyhjä onko kaikki sen viereiset suunnat tyhjiä tai niitä ei ole
          // kuten reunakohdissa.

          if (board.at(y).at(x) == 0)
          {
              if (up ==0 or down == 0 or left == 0 or right == 0)
              {
                  return true;
              }
          }
          else
          {
              if ((up == 0 or up == NOT_EXSIST) and (down == 0 or down == NOT_EXSIST)
                 and (right == 0 or right == NOT_EXSIST) and (left == 0 or left ==NOT_EXSIST))
              {
                  return true;
              }
          }

      }
  }
    return false;
}

// Tutkii onko peli voitettu. Palauttaa true jos pelivoitettu.
//

bool is_winning(const vector<vector<int>> board)
{
    // Luodaan sisäkkäisiä for looppeja, jotta saadaan tarkasteltua jokainen ruutu
    // Verrataan aina yhtä ruutua kerrallaan jokaiseen muuhun ruutuun sekä vaaka, että
    // pystyrivillä. Jos löytyy tyhjiä jatketaan, mutta jos samoja ruutuja palautetaan false.


    for (unsigned int y = 0; y < BOARD_SIDE; ++y)
    {


    for (unsigned int i = 0; i < BOARD_SIDE; ++i)
    {
        for ( unsigned int j = i+1; j < BOARD_SIDE; ++j)
        {
            if (board.at(y).at(i)==0 or board.at(i).at(y) == 0)
            {
                continue;
            }
            if (board.at(y).at(i) == board.at(y).at(j) or board.at(i).at(y) == board.at(j).at(y))
            {

               return false;
            }
    }
    }
}
   return true;
}

// Poistetaan ruutuja, tulostetaan virheimoitukset, jos ruutu jo poistettu tai se on laudan ulkopuolella ja
// tutkitaan joka kierroksella onko käyttäjä  hävinnyt tai voittanut pelin. Tulostetaan voitto/häviö ilmoitukset.

void remove_element(vector<vector<int>> &board)
{
    bool claim = true;
    // Toistetaan ketjua niin kauan, kunnes peli on hävitty, voitettu tai syötetään q.
    while (claim == true)
    {
        string x;
        string y;
        cout << "Enter removable element (x, y): ";
        cin >> x;
        if (x == "q" or x == "Q")
        {
           cout << "Quitting" << endl;
           break;
         }
         cin >> y;
         if(stoi_with_check(x) == 0 or stoi_with_check(y) == 0
            or stoi_with_check(x) > BOARD_SIDE or stoi_with_check(y) > BOARD_SIDE)
        {
            cout << "Out of board"<< endl;
        }
         else if (board.at(stoi_with_check(y)-1).at(stoi_with_check(x)-1) ==0)
         {
             cout << "Already removed"<< endl;
         }
        else
        {
            board.at(stoi_with_check(y)-1).at(stoi_with_check(x)-1) = 0;
            print(board);
            if (is_loosing(board) == true)
            {
                cout << "You lost" << endl;
                claim = false;
            }
            if (is_winning(board) == true)
            {
                cout << "You won" << endl;
                claim = false;
            }

        }

    }

}

int main()
{
    vector <int> row;
    vector<vector<int>>board;
    make_board(board, row);
    remove_element(board);






    return 0;
}
