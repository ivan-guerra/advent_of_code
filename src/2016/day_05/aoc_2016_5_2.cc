#include <openssl/evp.h>

#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

std::string GetMD5Hash(const std::string& secret) {
    const EVP_MD* md = EVP_get_digestbyname("MD5");
    if (!md) {
        std::cerr << "error: unknown message digest" << std::endl;
        return "";
    }

    unsigned char md_value[EVP_MAX_MD_SIZE] = {0};
    unsigned int md_len = 0;
    EVP_MD_CTX* mdctx = EVP_MD_CTX_create();

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, secret.c_str(), secret.size());
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);

    std::ostringstream os;
    for (int i = 0; i < md_len; ++i) {
        os << std::setfill('0') << std::setw(2) << std::hex
           << static_cast<int>(md_value[i]);
    }
    return os.str();
}

bool StartsWithFiveZeroes(const std::string& hash) {
    if (hash.size() < 5) {
        return false;
    }

    for (int i = 0; i < 5; ++i) {
        if (hash[i] != '0') {
            return false;
        }
    }
    return true;
}

std::string GetPassword(const std::string& door_id, int target_length) {
    std::string password(target_length, '\0');
    std::bitset<8> chars_found(0xFF);

    int suffix = 0;
    std::string suffix_str;
    do {
        suffix_str = std::to_string(suffix);
        std::string hash = GetMD5Hash(door_id + suffix_str);
        if (StartsWithFiveZeroes(hash)) {
            if ((hash[5] >= '0') && (hash[5] <= '7')) {
                int index = hash[5] - '0';
                if (chars_found[index]) {
                    chars_found[index] = 0;
                    password[index] = hash[6];
                }
            }
        }
        suffix++;
    } while (chars_found != 0);
    return password;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    OpenSSL_add_all_digests(); /* Required to init the openssl lib. */

    std::ifstream input(argv[1]);
    std::string door_id;
    std::getline(input, door_id);

    static const int kTargetLength = 8;
    std::cout << "Answer: " << GetPassword(door_id, kTargetLength) << std::endl;

    EVP_cleanup();

    exit(EXIT_SUCCESS);
}
