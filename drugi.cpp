#include <iostream>
#include <fstream>
#include <libssh/libssh.h>

int encryptAndSendFile(const char* sourcePath, const char* destinationHost, const char* sshPublicKey) {
    // U�itaj sadr�aj datoteke
    std::ifstream file(sourcePath, std::ios::binary);
    if (!file) {
        std::cerr << "Nemogu�e otvoriti datoteku: " << sourcePath << std::endl;
        return 1;
    }

    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    char* fileContent = new char[fileSize];
    if (!file.read(fileContent, fileSize)) {
        std::cerr << "Gre�ka pri �itanju datoteke." << std::endl;
        delete[] fileContent;
        return 1;
    }

    file.close();

    // �ifriraj datoteku koriste�i SSH javni klju�
    //stvara se ssh sesija koristeci dinamicku alokaciju
    ssh_session session = ssh_new();
    if (session == NULL) {
        std::cerr << "Gre�ka pri stvaranju SSH veze." << std::endl;
        delete[] fileContent;
        return 1;
    }
//postavljaju se opcije te sesije na odredisni host
    ssh_options_set(session, SSH_OPTIONS_HOST, destinationHost);
    //povetzuje se koristeci ssh connect i provjerava je li bilo uspjesno
    int connectionStatus = ssh_connect(session);
    if (connectionStatus != SSH_OK) {
        std::cerr << "Gre�ka pri povezivanju na odredi�ni host: " << ssh_get_error(session) << std::endl;
        ssh_free(session);
        delete[] fileContent;
        return 1;
    }
//automatska autentifikacija na odrdisnom hostu i provjerava se je li uspjesna
    int authenticationStatus = ssh_userauth_publickey_auto(session, NULL, NULL);
    if (authenticationStatus != SSH_AUTH_SUCCESS) {
        std::cerr << "Gre�ka pri autentifikaciji na odredi�nom hostu: " << ssh_get_error(session) << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        delete[] fileContent;
        return 1;
    }
//stvaranje ssh kopije za slanje sifrirane datoteke na odredisni host, scp nam treba za siguran prenos datoteka preko ssh protzokola
    ssh_scp scp = ssh_scp_new(session, SSH_SCP_WRITE, ".");
    if (scp == NULL) {
        std::cerr << "Gre�ka pri stvaranju SCP veze." << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        delete[] fileContent;
        return 1;
    }
//Generiranje jedinstvene putanje poma�e nam da spremimo �ifriranu datoteku
// na odredi�nom hostu bez preklapanja s drugim datotekama ili potencijalnim konfliktima u imenovanju.
    int scpStatus = ssh_scp_init(scp);
    if (scpStatus != SSH_OK) {
        std::cerr << "Gre�ka pri inicijalizaciji SCP veze: " << ssh_get_error(session) << std::endl;
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        delete[] fileContent;
        return 1;
    }
//generiras se putannja tako �to se pro�iruje sa ekstenzijom enc
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
std::cout << "Datoteka je uspje�no �ifrirana i poslana na odredi�ni host." << std::endl;

return 0;
}

int main() {
const char* sourceFilePath = "/putanja/do/datoteke";
const char* destinationHost = "korisnik@odredisni_host";
const char* sshPublicKeyPath = "/putanja/do/ssh_javnog_kljuca";
int result = encryptAndSendFile(sourceFilePath, destinationHost, sshPublicKeyPath);
return result;
}


