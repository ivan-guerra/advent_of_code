#include <openssl/evp.h>

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

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

int GetSecretKey(const std::string& prefix, const std::string& target_prefix) {
  int suffix = 0;
  std::string md5_hash;
  std::string candidate = prefix;
  do {
    candidate = prefix + std::to_string(suffix);
    md5_hash = GetMD5Hash(candidate);
    suffix++;
  } while (md5_hash.substr(0, target_prefix.size()) != target_prefix);

  return (suffix - 1);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  OpenSSL_add_all_digests(); /* Required to init the openssl lib. */

  std::ifstream input(argv[1]);
  std::string secret_prefix;
  std::getline(input, secret_prefix);

  std::cout << "Answer: " << GetSecretKey(secret_prefix, "000000") << std::endl;

  EVP_cleanup();

  exit(EXIT_SUCCESS);
}
