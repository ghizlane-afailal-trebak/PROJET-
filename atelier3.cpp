#include <iostream>
#include <string>
using namespace std ; 
class Voiture {
private:
    string marque;
    string modele;
    int annee;
    float kilometrage;
    float vitesse;

public:
    // Constructeur par défaut
    Voiture() : marque("Inconnue"), modele("Inconnu"), annee(0), kilometrage(0.0), vitesse(0.0) {}

    // Constructeur avec paramètres
    Voiture(string m, string mod, int a, float km, float v) 
        : marque(m), modele(mod), annee(a), kilometrage(km), vitesse(v) {}

    // Méthode pour accélérer
    void accelerer(float valeur) {
        vitesse += valeur;
    }

    // Méthode pour freiner
    void freiner(float valeur) {
        vitesse -= valeur;
        if (vitesse < 0) {
            vitesse = 0;
        }
    }

    // Méthode pour avancer
    void avancer(float distance) {
        kilometrage += distance;
    }

    // Méthode pour afficher les informations
    void afficherInfo() const {
        cout << "Marque: " << marque << "\n"
                  << "Modèle: " << modele << "\n"
                  << "Année: " << annee << "\n"
                  << "Kilométrage: " << kilometrage << " km\n"
                  << "Vitesse: " << vitesse << " km/h\n";
    }

    // Destructeur
    ~Voiture() {
        cout << "La voiture " << marque << " " << modele << " est détruite.\n";
    }
};

int main() {
    // Exemple d'utilisation de la classe Voiture
    Voiture voiture1("Toyota", "Corolla", 2020, 150.5, 120.0);
    voiture1.afficherInfo();

    voiture1.accelerer(30.0);
    voiture1.afficherInfo();

    voiture1.freiner(50.0);
    voiture1.afficherInfo();

    voiture1.avancer(100.0);
    voiture1.afficherInfo();

    return 0;
}