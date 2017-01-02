//
// Created by louis on 23/10/16.
//

#ifndef RENDUOPENGL_FILEBUFFER_H
#define RENDUOPENGL_FILEBUFFER_H

#include <string>
#include <vector>

class FileBuffer {
public :
    FileBuffer(std::string filename);

    void setFilename(std::string filename);
    std::string getFilename();
    void addLine(std::string line);

    /** Ecrit les données dans le fichier associé au filebuffer */
    void writeData();
private :
    std::string filename;
    std::vector<std::string> lines;
};


#endif //RENDUOPENGL_FILEBUFFER_H
