#include <openssl/evp.h>

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
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

char GetFirstTripletChar(const std::string& s) {
    int window_size = 3;
    for (int i = 0; (s.size() - i) >= window_size; ++i) {
        if (s[i] == s[i + 1] && (s[i] == s[i + 2])) {
            return s[i];
        }
    }
    return '\0';
}

bool HasTargetQuintuplet(const std::string& s, char target) {
    int window_size = 5;
    std::string target_str(window_size, target);
    for (int i = 0; (s.size() - i) >= window_size; ++i) {
        if (target_str == s.substr(i, window_size)) {
            return true;
        }
    }
    return false;
}

bool HasQuintHash(const std::list<std::string>& hashes, char target) {
    for (const std::string& hash : hashes) {
        if (HasTargetQuintuplet(hash, target)) {
            return true;
        }
    }
    return false;
}

int GetOtpKeyIndex(const std::string& salt) {
    std::list<std::string> hashes;
    int index = 0;
    for (int i = 0; i < 1001; ++i) {
        std::string hash = GetMD5Hash(salt + std::to_string(index));
        hashes.push_back(hash);
        index++;
    }

    int num_keys = 0;
    int local_index = 0;
    const int kTargetNumKeys = 64;
    while (num_keys < kTargetNumKeys) {
        std::string hash = hashes.front();
        hashes.pop_front();

        char target_char = GetFirstTripletChar(hash);
        if (('\0' != target_char) && HasQuintHash(hashes, target_char)) {
            num_keys++;
        }

        std::string new_hash = GetMD5Hash(salt + std::to_string(index));
        hashes.push_back(new_hash);
        index++;
        local_index++;
    }

    return (local_index - 1);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    OpenSSL_add_all_digests(); /* Required to init the openssl lib. */

    std::ifstream input(argv[1]);
    std::string salt;
    std::getline(input, salt);

    std::cout << "Answer: " << GetOtpKeyIndex(salt) << std::endl;

    EVP_cleanup();

    exit(EXIT_SUCCESS);
}
