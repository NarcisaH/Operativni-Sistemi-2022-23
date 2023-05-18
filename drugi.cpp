#include <iostream>
#include <fstream>
#include <libssh/libssh.h>

int encryptAndSendFile(const char* sourcePath, const char* destinationHost, const char* sshPublicKey) {
    // Uèitaj sadržaj datoteke
    std::ifstream file(sourcePath, std::ios::binary);
    if (!file) {
        std::cerr << "Nemoguæe otvoriti datoteku: " << sourcePath << std::endl;
        return 1;
    }

    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    char* fileContent = new char[fileSize];
    if (!file.read(fileContent, fileSize)) {
        std::cerr << "Greška pri èitanju datoteke." << std::endl;
        delete[] fileContent;
        return 1;
    }

    file.close();

    // Šifriraj datoteku koristeæi SSH javni kljuè
    //stvara se ssh sesija koristeci dinamicku alokaciju
    ssh_session session = ssh_new();
    if (session == NULL) {
        std::cerr << "Greška pri stvaranju SSH veze." << std::endl;
        delete[] fileContent;
        return 1;
    }
//postavljaju se opcije te sesije na odredisni host
    ssh_options_set(session, SSH_OPTIONS_HOST, destinationHost);
    //povetzuje se koristeci ssh connect i provjerava je li bilo uspjesno
    int connectionStatus = ssh_connect(session);
    if (connectionStatus != SSH_OK) {
        std::cerr << "Greška pri povezivanju na odredišni host: " << ssh_get_error(session) << std::endl;
        ssh_free(session);
        delete[] fileContent;
        return 1;
    }
//automatska autentifikacija na odrdisnom hostu i provjerava se je li uspjesna
    int authenticationStatus = ssh_userauth_publickey_auto(session, NULL, NULL);
    if (authenticationStatus != SSH_AUTH_SUCCESS) {
        std::cerr << "Greška pri autentifikaciji na odredišnom hostu: " << ssh_get_error(session) << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        delete[] fileContent;
        return 1;
    }
//stvaranje ssh kopije za slanje sifrirane datoteke na odredisni host, scp nam treba za siguran prenos datoteka preko ssh protzokola
    ssh_scp scp = ssh_scp_new(session, SSH_SCP_WRITE, ".");
    if (scp == NULL) {
        std::cerr << "Greška pri stvaranju SCP veze." << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        delete[] fileContent;
        return 1;
    }
//Generiranje jedinstvene putanje pomaže nam da spremimo šifriranu datoteku
// na odredišnom hostu bez preklapanja s drugim datotekama ili potencijalnim konfliktima u imenovanju.
    int scpStatus = ssh_scp_init(scp);
    if (scpStatus != SSH_OK) {
        std::cerr << "Greška pri inicijalizaciji SCP veze: " << ssh_get_error(session) << std::endl;
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        delete[] fileContent;
        return 1;
    }
//generiras se putannja tako što se proširuje sa ekstenzijom enc
    std::string encryptedFilePath = std::string(sourcePath) + ".enc";
    ssh_scp_push_file(scp, encryptedFilePath.c_str(), fileSize, S_IRUSR | S_IWUSR);
    ssh_scp_write(scp, fileContent, fileSize);
    ssh_scp_close(scp);
//sifrirana datoteka se postavlja na scp sesiju, specificira se putanja, velicina i sadrzaj datoteke
    // Oslobodi resurse
    delete[] fileContent;
    ssh_scp_free(scp
    continued:
ssh_disconnect(session);
ssh_free(session);
std::cout << "Datoteka je uspješno šifrirana i poslana na odredišni host." << std::endl;

return 0;
}

int main() {
const char* sourceFilePath = "/putanja/do/datoteke";
const char* destinationHost = "korisnik@odredisni_host";
const char* sshPublicKeyPath = "/putanja/do/ssh_javnog_kljuca";
int result = encryptAndSendFile(sourceFilePath, destinationHost, sshPublicKeyPath);
return result;
}


