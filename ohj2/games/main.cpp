/*  COMP.CS.100 Project 2: GAME STATISTICS
 *
 * Pelitilastot
 *
 * kuvaus:
 *    Ohjelma lukee tekstitiedoston pelitilastot ja tallentaa sen tiedot.
 * Pelitilastoista pystyy hakemaan tietoa ja muokkamaan niitä. Jotta
 * tiedot saadaan luettua tiedostossa tulee tiedot ilmoittaa
 * pelin nimi;pelaajan nimi;pisteet muodossa.
 *    Aluksi käyttäjältä kysytään, mikä tiedosto avataan ja tarkastellaan,
 * voidaanko tekstitiedosto avata. Sen jälkeen ohjelma kysyy käyttäjältä
 * syötteitä, jonka mukaan se tulostaa tekstitiedston tiedot. Toimet, joita
 * pelitilastoille voi tehdä ja niiden syötteet:
 * ALL_GAMES - Tulostaa kaikkien tilastoissa olevien pelien nimet
 * GAME <pelin nimi> - Tulostaa kyseisen pelin pelaajat ja heidän pisteensä.
 * ALL_PLAYERS - Tulostaa kaikki tilastoissa esiintyvät nimet.
 * PLAYER <pelaajan nimi> - Tulostaa kaikki kyseisen pelaajan pelaamat pelit.
 * ADD_GAME <pelin nimi> - Lisätään uusi peli tilastoihin
 * ADD_PLAYER <pelin nimi> <pelaajan nimi> <pisteet> - Lisätään uusi pelaaja
 * ja hänen pisteensä haluttuun peliin tai päivittää pelaajan pisteet.
 * REMOVE <pelaajan nimi> - Poistaa pelaajan ja hänen tiedot tilastoista.

 *
 * Ohjelman kirjoittaja
 * Nimi: Silja Kantanen
 * Opiskelijanumero: 50343681
 * Käyttäjätunnus: cxsika
 * E-mail: silja.kantanen@tuni.fi
 * */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cctype>

//tietorakenne
using GAMES = std::map<std::string, std::map<std::string, int>> ;

//merkkijonovakioita
const std::string FILE_NOT_READ_TEXT = "Error: File could not be read.",
                  INPUT_FILE_PROMPT = "Give a name for input file: ",
                  FILE_FORMAT_INVALID_TEXT = "Error: Invalid format in file.";

// Casual split func, if delim char is between "'s, ignores it.
std::vector<std::string> split( const std::string& str, char delim = ';' )
{
    std::vector<std::string> result = {""};
    bool inside_quatation = false;
    for ( auto current_char : str )
    {
        if ( current_char == '"' )
        {
            inside_quatation = not inside_quatation;
        }
        else if ( current_char == delim and not inside_quatation )
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    if ( result.back() == "" )
    {
        result.pop_back();
    }
    return result;
}


// Funktio tarkistaa onko vektorissa kolme osaa ja palauttaa totuusarvon.
bool line_is_ok(std::vector<std::string> const &line_parts)
{
    // vektori on kolmiosainen = ; oli kaksi kpl

    //pelin ja pelaajan nimi eivät ole tyhjiä
    return line_parts.size() == 3
       && !line_parts.at(0).empty()
       && !line_parts.at(1).empty();
}

// Funktio avaa käyttäjän syöttämän tiedoston ja tarkistaa voiko tiedostoa avata.
// Jos tiedosto pysytään avaamaan, tallennetaan tiedot valittuun
// tietorakenteeseen. Palauttaa totuusarvon riippuen siitä, onko tiedosto
// onnistuttu avaamaan oikein ja onko tiedostossa tiedot vaaditulla tavalla
// kirjoitteuna.
bool read_file_input(GAMES &scoreboard)
{
    // haetaan tiedostonnimi
    std::string filename;
    std::cout << INPUT_FILE_PROMPT;
    getline(std::cin,filename);

    // avataan tiedosto
    std::ifstream file(filename);

    if (!file)
    {
        std::cout << FILE_NOT_READ_TEXT << std::endl;
        return false;
    }

    // käydään tiedosto läpi ja tarkastellaan syöte
    std::string line = "";
    std::vector< std::string> line_parts;
    while(std::getline(file, line))
    {
        line_parts = split(line);

        if(!line_is_ok(line_parts))
        {
            std::cout << FILE_FORMAT_INVALID_TEXT << std::endl;
            file.close();
            return false;
        }

        std::string gamename = line_parts.at(0),
                    player = line_parts.at(1),
                    points_str = line_parts.at(2);

        // tarkistaa onko peli tietokannassa
        if (scoreboard.find(gamename) == scoreboard.end())
        {
            scoreboard.insert({gamename, {}});
        }

        //lisää pelaajan pisteet tietokantaan
        scoreboard.at(gamename).insert({player, std::stoi(points_str)});
    }
    return true;
}

//Funktio tulostaa kaikki pelitilastossa olevat peli.
void print_all_games(GAMES const &scoreboard)
{
    std::cout << "All games in alphabetical order:" << std::endl;

    for(auto entry : scoreboard)
    {
        std::cout << entry.first << std::endl;
    }
}

// Funktio ulostaa käyttäjän syöttämän pelin pelaajat ja heidän pisteensä.
void print_game(GAMES const &scoreboard, const std::string &game_name)
{
    std::map < int, std::string> points;
    std::string name;

    if (scoreboard.find(game_name) != scoreboard.end())
    {
        for (auto entry : scoreboard.at(game_name))
        {
            if (points.find(entry.second) != points.end())
            {
                name = points.at(entry.second)+ ", " + entry.first;
                points.at(entry.second) = name;
            }
            else
            {

                points.insert({entry.second, entry.first});

            }
        }
        std::cout << "Game " << game_name;
        std::cout << " has these scores and players, listed in ";
        std::cout << "ascending order:" << std::endl;
        for (auto playerspoints : points)
        {
            std::cout << playerspoints.first << " : " ;
            std::cout << playerspoints.second << std::endl;
        }

    }
    else
    {
        std::cout << "Error: Game could not be found." << std::endl;
    }

}

// Funktio tulostaa kaikki pelitilastossa olevat pelaajat
void print_players(GAMES scoreboard)
{
    // Haetaan pelitilastoista pelaajat ja lisätään ne settiin, jos ne eivät
    // ole jo mainittu setissä
    std::set < std::string > players;
    for (auto entry : scoreboard)
    {
        for (auto playerpoint : entry.second)
        {
            if (players.find(playerpoint.first) == players.end() )
            {
                players.insert(playerpoint.first);
            }
        }

    }

    std::cout << "All players in alphabetical order:" << std::endl;
    for (std::string player : players)
    {
        std::cout << player << std::endl;
    }
}

// Funktiolla tulostetaan pelit, jossa käyttäjän syöttämä pelaaja on
// pelannut.
void print_player(GAMES scoreboard, std::string player_name)
{
    // Pelaajan pelit, joissa pelaa lisätään settiin.
    std::set < std::string > games;
    for (auto entry : scoreboard)
    {
        if (entry.second.find(player_name) != entry.second.end())
        {
            games.insert(entry.first);
        }

        else
        {
            continue;
        }
    }
    // Jos setti jää tyhjäksi, pelaaja ei ole tilastoissa
    if (games.size() == 0)
    {
        std::cout << "Error: Player could not be found." << std::endl;
    }

    // Muuten tulosetetaan pelit, jotka on lisätty settiin.
    else
    {
        std::cout << "Player " << player_name;
        std::cout << " playes the following games:" << std::endl;

        for (std::string game : games)
        {
            std::cout << game << std::endl;
        }

    }

}

// Funktio korottaa sanan kaikki kirjaimet isoksi.
void to_upper_word(std::string &word)
{
    int lenght = word.length();

    for (int i = 0; i < lenght; i++)
    {
      word[i]= toupper(word[i]);

    }
}

//Funktiolla lisätään käyttäjän syöttämä uusi peli pelitilastojen mappiin.
void add_game(GAMES &scoreboard, std::string game_name)
{
    if (scoreboard.find(game_name) == scoreboard.end())
    {
        scoreboard.insert({game_name, {}});
        std::cout << "Game was added." << std::endl;
    }

    else
    {
        std::cout << "Error: Already exists." << std::endl;
    }

}

// Funktiolla lisätäään pelaaja ja sen pisteet haluttuun peliin.
// Jos pelaaja on jo halutussa pelissä, päivitetään pisteet uuteen.
void add_player(GAMES &scoreboard, std::string game_name,
                std::string player_name, int points)
{
    // Tarkastetaan löytyykö peli pelitilastoista
    if (scoreboard.find(game_name) != scoreboard.end())
    {
        // Tarkastetaan löytyykö pelaaja jo pelistä.
        if (scoreboard.at(game_name).find(player_name) !=
                scoreboard.at(game_name).end())
        {
            scoreboard.at(game_name).at(player_name) = points;
            std::cout << "Player was added." << std::endl;
        }

        else
        {
            scoreboard.at(game_name).insert({player_name, points});
            std::cout << "Player was added." << std::endl;
        }

    }

    else
    {
       std::cout << "Error: Game could not be found." << std::endl;
    }

 }

// Funktio poistaa käyttäjän syöttämän pelaajan kaikista peleistä.
void remove_player(GAMES &scoreboard, std::string player_name)
{
    // Tarkastetaan löytyykö pelaajaa pelitilastoista, jos
    // löytyy poistetaan pelaaja ja muutetaan lopputulos teksti.
    std::string result = "Error: Player could not be found.";
    for (auto &pair : scoreboard)
    {
        if (pair.second.find(player_name) != pair.second.end() )
        {
            pair.second.erase(player_name);
            result = "Player was removed from all games.";
        }
    }

    std::cout << result << std::endl;
}

int main()
{
    // tietorakenne

    GAMES scoreboard;

    //luetaan tiedostosyöte
    if (!read_file_input(scoreboard))
    {
        return EXIT_FAILURE;
    }

    std::string user_input = "";
    std::vector<std::string > parts;


    while (true)
    {
        std::cout << "games> ";
        std::getline(std::cin, user_input);
        parts = split(user_input, ' ');

        // syötteet korjataan isoiksi kirjaimiksi
        to_upper_word(parts.at(0));

        if (parts.at(0) == "QUIT")
        {
            break;
        }

        else if(parts.at(0) == "ALL_GAMES")
        {
            print_all_games(scoreboard);
        }

        else if(parts.at(0) == "GAME" && parts.size() >=2)
        {
            print_game(scoreboard, parts.at(1));
        }

        else if(parts.at(0) == "ALL_PLAYERS")
        {
            print_players(scoreboard);
        }

        else if(parts.at(0) == "PLAYER" && parts.size() >= 2)
        {
            print_player(scoreboard, parts.at(1));
        }

        else if (parts.at(0) == "ADD_GAME" && parts.size() >= 2)
        {
           add_game(scoreboard, parts.at(1));
        }

        else if (parts.at(0) == "ADD_PLAYER" && parts.size() == 4)
        {
            int points = stoi(parts.at(3));
            add_player(scoreboard, parts.at(1), parts.at(2), points);
        }

        else if (parts.at(0) == "REMOVE" && parts.size() == 2)
        {
            remove_player(scoreboard, parts.at(1));
        }

        else
        {
            std::cout << "Error: Invalid input." << std::endl;
        }

    }

    return EXIT_SUCCESS;
}
