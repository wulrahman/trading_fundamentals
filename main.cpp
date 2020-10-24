//
//  main.c
//  Bot
//
//  Created by Waheed Ul Rahman on 10/24/20.
//

#include "main.h"

enum type_T { open_T, high_T, low_T, close_T, volume_T, market_cab_T };

struct price_structure {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
    double market_cab;
};

double get_value(price_structure source, type_T type) {
    
    switch (type) {
        case open_T:
            return source.open;
            break;
        case high_T:
            return source.high;
            break;
        case low_T:
            return source.low;
            break;
        case close_T:
            return source.close;
            break;
        case volume_T:
            return source.volume;
            break;
        case market_cab_T:
            return source.market_cab;
            break;
        default:
            return EXIT_FAILURE;
            
    }
}

std::vector<double> get_row(std::vector<price_structure> source, type_T type) {
    std::vector<double> return_t;
    for (std::vector<price_structure>::const_iterator price = source.begin(); price != source.end(); ++price) {
        return_t.insert(return_t.end(), get_value((*price), type));
    }
    return return_t;
}

std::vector<double> sma(size_t const lenght, std::vector<double> source) {
    
    size_t i = 0;
    std::vector<double> sma;
    double temp;

    if(lenght < source.size()) {
        for(std::vector<double>::const_iterator index = source.begin(); index != source.begin() + lenght; index++) {
            temp = 0;
            sma.insert(sma.end(), temp);
            std::cout << temp << std::endl;
        }
        for(std::vector<double>::const_iterator index = source.begin() + lenght; index != source.end(); index++) {
            std::vector<double> split(source.begin()+i, source.begin() + lenght + i);
            temp = std::accumulate(split.begin(),split.end(),0.000000)/lenght;
            sma.insert(sma.end(), temp);
            std::cout << temp << std::endl;
            i++;
        }
        return sma;
    }
    else {
        return (std::vector<double>) EXIT_FAILURE;
    }
    
}

std::vector<double> obv(std::vector<price_structure> source) {
    
    size_t i = 0;
    std::vector<double> obv;
    double temp = 0;

    if(1 < source.size()) {
        for(std::vector<price_structure>::const_iterator index = source.begin(); index != source.end(); index++) {
            if(i == 0) {
                temp = 0;
            }
            else if((*index).close == source.at(i-1).close){
                temp = obv.back();
            }
            else if((*index).close  > source.at(i-1).close) {
                temp = obv.back() + (*index).volume;
            }
            else if((*index).close  < source.at(i-1).close) {
                temp = obv.back() + -1*(*index).volume;
            }
            obv.insert(obv.end(), temp);
            std::cout << temp << std::endl;
            i++;
        }
        return obv;
    }
    else {
        return (std::vector<double>) EXIT_FAILURE;
    }
    
}

std::vector<double> sfi(std::vector<price_structure> source) {
    
    size_t i = 0;
    std::vector<double> smi;
    std::vector<double> positive_smi;
    std::vector<double> negitive_smi;


    double temp = 0;
    double temp_typical = 0;
    double old_typical = 0;


    if(14 < source.size()) {
        for(std::vector<price_structure>::const_iterator index = source.begin(); index != source.end(); index++) {
            temp_typical = ((*index).high + (*index).low + (*index).close)/3 * (*index).volume;
            
            if(temp_typical > old_typical) {
                if(positive_smi.size() == 14 ) {
                    positive_smi.erase(positive_smi.begin());
                }
                positive_smi.insert(positive_smi.end(), temp_typical);
            }
            else if(temp_typical < old_typical) {
                if(negitive_smi.size() == 14 ) {
                    negitive_smi.erase(negitive_smi.begin());
                }
                negitive_smi.insert(negitive_smi.end(), temp_typical);
            }
            
            if(positive_smi.size() < 14 || negitive_smi.size() < 14) {
                temp = 0;
            }
            else {
                temp = 100-(100)/(1+(std::accumulate(positive_smi.begin(),positive_smi.end(),0.000000)/std::accumulate(negitive_smi.begin(),negitive_smi.end(),0.000000)));
            }
            old_typical = temp_typical;
            
            smi.insert(smi.end(), temp);
            std::cout << temp << std::endl;
            i++;
        }
        return smi;
    }
    else {
        return (std::vector<double>) EXIT_FAILURE;
    }
    
}



std::vector<double> ema(size_t const lenght, std::vector<double> source) {
    
    size_t i = 0;
    std::vector<double> ema;
    double temp = 0;
    double alpah = 2.0/(lenght + 1.0);

    if(1 < source.size()) {
        for(std::vector<double>::const_iterator index = source.begin(); index != source.end(); index++) {
            if(i == 0) {
                temp = ((*index)*alpah);
            }
            else {
                temp = ((*index)*alpah) + ema.back() * (1-alpah);
            }
            ema.insert(ema.end(), temp);
            std::cout << temp << std::endl;
            i++;
        }
        return ema;
    }
    else {
        return (std::vector<double>) EXIT_FAILURE;
    }
    
}

std::vector<std::string> split(const std::string& string, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t previous = 0, position = 0;
    do {
        position = string.find(delimiter, previous);
        if (position == std::string::npos) {
            position = string.length();
        }
        std::string token = string.substr(previous, position-previous);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        previous = position + delimiter.length();
    }
    while (position < string.length() && previous < string.length());
    return tokens;
}

std::string remove_punctuation(std::string string) {
    string.erase(
        std::remove_if(string.begin(), string.end(), [](char x) {
                return ispunct(x) || isspace(x);
        }),
    string.end());
    return string;
}

double string_to_double(std::string string) {
    return ::atof(string.c_str());
}

int main(int argc, char * argv[]) {
    
    std::ifstream myfile("/Users/wulrahman/Downloads/AlgoBot-master/Bot/Bot/ETChis.txt");
    std::string line;
    std::vector<price_structure> prices;
    std::vector<std::string> temp;
//    std::cout.precision(17);

    if (myfile.is_open()) {
        std::getline(myfile,line);
        while (std::getline(myfile,line)) {
            temp = split(line,"\t");
            prices.insert(prices.end(), {
                .date = temp[0],
                .open = string_to_double(temp[1]),
                .high = string_to_double(temp[2]),
                .low = string_to_double(temp[3]),
                .close = string_to_double(temp[4]),
                .volume = string_to_double(remove_punctuation(temp[5])),
                .market_cab = string_to_double(remove_punctuation(temp[6]))
            });
        }
        myfile.close();
    }
    else {
        std::cout << "Unable to open file";
    }
    
    std::vector<double> closes = get_row(prices, close_T);

    sfi(prices);

    return EXIT_SUCCESS;
}
