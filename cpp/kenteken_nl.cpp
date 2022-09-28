#include "./kenteken_nl.h"
#include "./licence.h"
#include "./constants.h"

/*
    Finds the first letter part
*/
string first_letter_part(RecognitionResult test) {

    if (!test.first.empty() and !isdigit(test.first.at(0)))
        return test.first;
    else if (!test.second.empty() and !isdigit(test.second.at(0)))
        return test.second;
    else if (!test.third.empty())
        return test.third;
    else
        return "";
}

/*
    First letter is an M for motorbikes
*/
bool is_motor_licence(RecognitionResult test) {

    string firstLetters = first_letter_part(test);

    if (firstLetters.size() > 0
        and strncmp("M", firstLetters.substr(0, 1).c_str(), 1) == 0)
        return true;
    else
        return false;
}

/*
    First letter is V for bedrijfswagens up to 3.500 kg
*/
bool is_bedrijfswagen(RecognitionResult test) {
    string firstLetters = first_letter_part(test);

    if (firstLetters.size() > 0
        and strncmp("V", firstLetters.substr(0, 1).c_str(), 1) == 0)
        return true;
    else
        return false;
}

/*
    First letter is B for bedrijfswagens above 3.500 kg
*/
bool is_bedrijfswagen_heavy(RecognitionResult test) {
    string firstLetters = first_letter_part(test);

    if (firstLetters.size() > 0
        and strncmp("B", firstLetters.substr(0, 1).c_str(), 1) == 0)
        return true;
    else
        return false;
}

/*
    First letter is W for aanhangers
*/
bool is_aanhanger(RecognitionResult test) {
    string firstLetters = first_letter_part(test);

    if (firstLetters.size() > 0
        and strncmp("W", firstLetters.substr(0, 1).c_str(), 1) == 0)
        return true;
    else
        return false;
}

/*
    First letter is O for opleggers (which is not in the vocabulary used by the recognition so this will always return false
    and an oplegger will be categorized as a bedrijfsvoertuig instead because the O will most likely be recognized as a V)
*/
bool is_oplegger(RecognitionResult test) {
    string firstLetters = first_letter_part(test);

    if (firstLetters.size() > 0
        and strncmp("O", firstLetters.substr(0, 1).c_str(), 1) == 0)
        return true;
    else
        return false;
}

/*
    First letters are GV for grensverkeer
*/
bool is_grensverkeer(RecognitionResult test) {
    string firstLetters = first_letter_part(test);

    if (firstLetters.size() > 1
        and strncmp("GV", firstLetters.substr(0, 2).c_str(), 2) == 0)
        return true;
    else
        return false;
}

/*
    First letter is T for landbouwvoertuigen
*/
bool is_landbouwvoertuig(RecognitionResult test) {
    string firstLetters = first_letter_part(test);

    if (firstLetters.size() > 0
        and strncmp("T", firstLetters.substr(0, 1).c_str(), 1) == 0)
        return true;
    else
        return false;
}

/*
    First letter is L for landbouwaanhangwagens
*/
bool is_landbouwaanhangwagen(RecognitionResult test) {
    string firstLetters = first_letter_part(test);

    if (firstLetters.size() > 0
        and strncmp("L", firstLetters.substr(0, 1).c_str(), 1) == 0)
        return true;
    else
        return false;
}

bool is_scooter(RecognitionResult test) {
    string firstLetters = first_letter_part(test);
    int sizeFirstPart = int(test.first.size() + test.second.size());

    // A brommer has a yellow plate starting with D
    if (firstLetters.size() > 0
        and strncmp("D", firstLetters.substr(0, 1).c_str(), 1) == 0
        and sizeFirstPart >= N_PARTS)
        return true;

    // Snorfiets has a blue plate starting with F
    if (firstLetters.size() > 0
        and strncmp("F", firstLetters.substr(0, 1).c_str(), 1) == 0
        and sizeFirstPart >= N_PARTS)
        return true;
    else
        return false;
}

bool is_car_licence(RecognitionResult test) {
    if (is_motor_licence(test) || is_bedrijfswagen(test) || is_bedrijfswagen_heavy(test)
        || is_aanhanger(test) || is_oplegger(test) || is_grensverkeer(test)
        || is_landbouwvoertuig(test) || is_landbouwaanhangwagen(test) || is_scooter(test))
        return false;
    return true;
}