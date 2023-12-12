#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>

std::map<std::string, std::map<std::string, double>> type_effectiveness = {
    {"Fire", {{"Fire", 0.5}, {"Water", 0.5}, {"Grass", 2.0}, {"Electric", 1.0}}},
    {"Water", {{"Fire", 2.0}, {"Water", 0.5}, {"Grass", 0.5}, {"Electric", 1.0}}},
    {"Grass", {{"Fire", 0.5}, {"Water", 2.0}, {"Grass", 0.5}, {"Electric", 1.0}}},
    {"Electric", {{"Fire", 1.0}, {"Water", 2.0}, {"Grass", 0.5}, {"Electric", 0.5}}}
};

class Pokemon {
private:
    std::string name;
    std::string type;
    int level;
    int attack;
    int defense;
    int spAttack;
    int spDefense;
    int maxHealth;
    int actualHealth;
    bool isKnockedOut;

public:
    Pokemon(std::string name, std::string type, int level = 5) {
        this->name = name;
        this->type = type;
        this->level = level;
        this->attack = rand() % 9 + 1;
        this->defense = rand() % 9 + 1;
        this->spAttack = rand() % 9 + 1;
        this->spDefense = rand() % 9 + 1;
        this->maxHealth = (rand() % 5 + 5) * level;
        this->actualHealth = this->maxHealth;
        this->isKnockedOut = false;
    }

    void revive(bool revive) {
        if (this->actualHealth == 0 && revive && this->isKnockedOut) {
            this->actualHealth = (this->maxHealth > 15) ? 15 : this->maxHealth;
            this->isKnockedOut = false;
            std::cout << this->name << " was revived with a total of " << this->actualHealth << std::endl;
        }
    }

    void cure(int amount) {
        if (this->actualHealth == 0 && this->isKnockedOut) {
            std::cout << "The pokemon is knocked, revive him first" << std::endl;
            return;
        } else {
            if (this->gainHealth(amount) >= this->maxHealth) {
                this->actualHealth = this->maxHealth;
            }
        }
        std::cout << this->name << " was healed, his actual health is: " << this->actualHealth << std::endl;
    }

    void knockOut() {
        this->isKnockedOut = true;
        if (this->actualHealth != 0) {
            this->actualHealth = 0;
        }
        std::cout << this->name << " is knocked out" << std::endl;
    }

    void loseHealth(int amount) {
        if (this->actualHealth - amount <= 0) {
            this->actualHealth = 0;
            this->knockOut();
        } else {
            this->actualHealth -= amount;
            std::cout << this->name << " lost " << amount << " points of health. Its actual health is " << this->actualHealth << std::endl;
        }
    }

    int gainHealth(int amount) {
        if (this->actualHealth + amount >= this->maxHealth) {
            this->actualHealth = this->maxHealth;
        } else {
            this->actualHealth += amount;
        }
        std::cout << this->name << " gained " << amount << " points of health. Its actual health is " << this->actualHealth << std::endl;
        return this->actualHealth;
    }

    std::string checkAttackEffectiveness(Pokemon rival) {
        double effectiveness = type_effectiveness[this->type][rival.getType()];
        if (effectiveness > 1.0) {
            return "Super Effective";
        } else if (effectiveness < 1.0) {
            return "Not Very Effective";
        } else {
            return "Normal Effect";
        }
    }

    void attackRival(Pokemon &rival) {
        std::cout << "DEBUGGING ATTACK ===========================" << std::endl;
        std::string attackEffectiveness = this->checkAttackEffectiveness(rival);
        std::cout << "DEBUGGING ATTACK ===========================" << std::endl;
        double attackModifier = 1.0;
        if (attackEffectiveness == "Super Effective") {
            attackModifier = 2.0;
        } else if (attackEffectiveness == "Not Very Effective") {
            attackModifier = 0.5;
        }
        if (this->attack * attackModifier > rival.getDefense()) {
            int attackDamage = static_cast<int>(this->attack * attackModifier - rival.getDefense());
            rival.loseHealth(attackDamage);
            std::cout << this->name << " attacked " << rival.getName() << ". Dealt " << attackDamage << " points of damage to " << rival.getName() << std::endl;
        } else {
            std::cout << this->name << " attack didn't deal damage to " << rival.getName() << std::endl;
        }
    }

    std::string getName() const {
        return this->name;
    }

    std::string getType() const {
        return this->type;
    }

    int getDefense() const {
        return this->defense;
    }

    bool getIsKnockedOut() const {
        return this->isKnockedOut;
    }

    friend std::ostream &operator<<(std::ostream &os, const Pokemon &pokemon) {
        os << "A new " << pokemon.name << " with level: " << pokemon.level << "and type "<<
        pokemon.type << " was created. It's initial stats are: Attack: " << pokemon.attack <<
        ", Defense: " <<pokemon.defense << ", Special Attack: " <<pokemon.spAttack << ", Special Defense: " <<
        pokemon.spDefense << " Max health: " << pokemon.maxHealth << "and Actual health" << pokemon.actualHealth << std::endl;
        return os;
    }
};

class Trainer {
private:
    std::vector<Pokemon> pokemons;
    int potions;
    int revives;
    size_t currentPokemon;
    std::string name;

public:
    Trainer(std::vector<Pokemon> pokemonList, int numPotions, int numRevives, std::string trainerName) {
        this->pokemons = pokemonList;
        this->potions = numPotions;
        this->revives = numRevives;
        this->currentPokemon = 0;
        this->name = trainerName;
    }

    void newInitial(size_t newPokemon) {
        if (newPokemon < this->pokemons.size() && newPokemon >= 0) {
            if (this->pokemons[newPokemon].getIsKnockedOut()) {
                std::cout << this->pokemons[newPokemon].getName() << " is knocked out. Can't be selected to fight" << std::endl;
            } else if (newPokemon == this->currentPokemon) {
                std::cout << this->pokemons[newPokemon].getName() << " is already the active pokemon" << std::endl;
            } else {
                this->currentPokemon = newPokemon;
                std::cout << "I choose you " << this->pokemons[newPokemon].getName() << "!" << std::endl;
            }
        }
    }

    void usePotion() {
        if (this->potions > 0) {
            std::cout << "You used a potion on " << this->pokemons[this->currentPokemon].getName() << "." << std::endl;
            this->pokemons[this->currentPokemon].gainHealth(20);
            this->potions--;
        } else {
            std::cout << "You don't have any more potions" << std::endl;
        }
    }

    void useRevive() {
        if (this->revives > 0 && this->pokemons[this->currentPokemon].getIsKnockedOut()) {
            std::cout << "You used a revive on " << this->pokemons[this->currentPokemon].getName() << "." << std::endl;
            this->pokemons[this->currentPokemon].gainHealth(20);
            this->revives--;
        } else {
            std::cout << "You don't have any more revives or the pokemon is not knocked out yet" << std::endl;
        }
    }

    void attackOtherTrainer(Trainer &otherTrainer) {
        Pokemon myPokemon = this->pokemons[this->currentPokemon];
        Pokemon rivalPokemon = otherTrainer.pokemons[otherTrainer.currentPokemon];
        std::cout << "DEBUG===== " << myPokemon.getName() << std::endl;
        std::cout << "DEBUG===== " << rivalPokemon.getName() << std::endl;
        myPokemon.attackRival(rivalPokemon);
    }

    friend std::ostream &operator<<(std::ostream &os, const Trainer &trainer) {
        os << trainer.name << " trainer has these pokemons: " << std::endl;
        for (const Pokemon &pokemon : trainer.pokemons) {
            os << pokemon << std::endl;
        }
        os << "The initial pokemon is " << trainer.pokemons[trainer.currentPokemon].getName();
        return os;
    }
};

int main() {
    // Setting seed for random number generation
    std::srand(std::time(0));

    // Six pokemon are made with different levels. (If no level is given, it is level 5)
    Pokemon pikachu("Pikachu", "Electric");
    std::cout << pikachu << std::endl;

    Pokemon a("Charmander", "Fire", 7);
    Pokemon b("Squirtle", "Water");
    Pokemon c("Lapras", "Water", 9);
    Pokemon d("Bulbasaur", "Grass", 10);
    Pokemon e("Vulpix", "Fire");
    Pokemon f("Staryu", "Water", 4);

    // Getting input to get the trainers' names and letting them select the Pokemon they want.
    std::string firstTrainer, secondTrainer;
    std::cout << "Welcome to the world of Pokemon. Please enter a name for player one and hit enter. ";
    std::cin >> firstTrainer;
    std::cout << "Hi, " << firstTrainer << "! Welcome! Let's find you an opponent. Enter a name for the second player. ";
    std::cin >> secondTrainer;

    std::string choice;
    std::cout << "Hi, " << secondTrainer << "! Let's pick our Pokemon! " << firstTrainer << ", would you like a Level 7 Charmander, or a Level 7 Squirtle? " << secondTrainer << " will get the other one. Type either 'Charmander' or 'Squirtle'. ";
    std::cin >> choice;

    while (choice != "Charmander" && choice != "Squirtle") {
        std::cout << "Whoops, it looks like you didn't choose 'Charmander' or 'Squirtle'. Try selecting one again! ";
        std::cin >> choice;
    }

    std::vector<Pokemon> firstTrainerPokemons, secondTrainerPokemons;

    if (choice == "Charmander") {
        firstTrainerPokemons.push_back(a);
        secondTrainerPokemons.push_back(b);
    } else {
        firstTrainerPokemons.push_back(b);
        secondTrainerPokemons.push_back(a);
    }

    // Selecting the second pokemon
    std::cout << secondTrainer << ", would you like a Level 9 Lapras, or a Level 10 Bulbasaur? " << firstTrainer << " will get the other one. Type either 'Lapras' or 'Bulbasaur'. ";
    std::cin >> choice;

    while (choice != "Lapras" && choice != "Bulbasaur") {
        std::cout << "Whoops, it looks like you didn't choose 'Lapras' or 'Bulbasaur'. Try selecting one again! ";
        std::cin >> choice;
    }

    if (choice == "Lapras") {
        firstTrainerPokemons.push_back(d);
        secondTrainerPokemons.push_back(c);
    } else {
        firstTrainerPokemons.push_back(c);
        secondTrainerPokemons.push_back(d);
    }

    // Selecting the third pokemon
    std::cout << firstTrainer << ", would you like a Level 5 Vulpix, or a Level 4 Staryu? " << secondTrainer << " will get the other one. Type either 'Vulpix' or 'Staryu'. ";
    std::cin >> choice;

    while (choice != "Vulpix" && choice != "Staryu") {
        std::cout << "Whoops, it looks like you didn't choose 'Vulpix' or 'Staryu'. Try selecting one again! ";
        std::cin >> choice;
    }

    if (choice == "Vulpix") {
        firstTrainerPokemons.push_back(e);
        secondTrainerPokemons.push_back(f);
    } else {
        firstTrainerPokemons.push_back(f);
        secondTrainerPokemons.push_back(e);
    }

    // Creating the Trainer objects with the given names and pokemon lists
    Trainer firstTrainerObj(firstTrainerPokemons, 3, 2, firstTrainer);
    Trainer secondTrainerObj(secondTrainerPokemons, 3, 1, secondTrainer);

    std::cout << "Let's get ready to fight! Here are our two trainers" << std::endl;

    std::cout << firstTrainerObj << std::endl;
    std::cout << secondTrainerObj << std::endl;

    // Testing attacking, giving potions, and switching pokemon. This could be built out more to ask for input
    firstTrainerObj.attackOtherTrainer(secondTrainerObj);
    secondTrainerObj.attackOtherTrainer(firstTrainerObj);
    secondTrainerObj.usePotion();
    firstTrainerObj.attackOtherTrainer(secondTrainerObj);
    secondTrainerObj.newInitial(0);
    secondTrainerObj.newInitial(1);

    return 0;
}
