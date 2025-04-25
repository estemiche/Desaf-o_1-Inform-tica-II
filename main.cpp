#include <QCoreApplication>
#include <QImage>
#include <iostream>
#include "bitwise_op.h"

using namespace std;

// Prototipos de funciones auxiliares
unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString archivoEntrada = "I_D_R6.bmp";      // Imagen corregida del Commit 10
    QString archivoSalida  = "I_D_R7.bmp";   // Imagen final recuperada

    int width = 0, height = 0;
    unsigned char* pixelData = loadPixels(archivoEntrada, width, height);
    if (!pixelData) {
        cerr << "Error al cargar la imagen '" << archivoEntrada.toStdString() << "'" << endl;
        return -1;
    }

    // Simular datos del paso anterior para mantener coherencia de salida
    int seed = 15;
    int n_pixels = 100;
    cout << "Semilla: " << seed << endl;
    cout << "Cantidad de pixeles leidos: " << n_pixels << endl;

    cout << "Aplicando rotacion inversa de 3 bits a la izquierda en cada canal..." << endl;

    int totalBytes = width * height * 3;
    for (int i = 0; i < totalBytes; ++i) {
        pixelData[i] = rotateLeftByte(pixelData[i], 3);
    }

    bool exportado = exportImage(pixelData, width, height, archivoSalida);
    cout << "Imagen BMP modificada guardada como " << archivoSalida.toStdString() << endl;
    cout << "Exportacion: " << (exportado ? "Exitosa" : "Fallida") << endl;

    delete[] pixelData;
    return 0;
}

unsigned char* loadPixels(QString input, int &width, int &height){
    QImage imagen(input);
    if (imagen.isNull()) {
        cerr << "Error: No se pudo cargar la imagen BMP '" << input.toStdString() << "'" << endl;
        return nullptr;
    }
    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    width  = imagen.width();
    height = imagen.height();

    int dataSize = width * height * 3;
    unsigned char* buf = new unsigned char[dataSize];
    for (int y = 0; y < height; ++y) {
        memcpy(buf + y * width * 3,
               imagen.scanLine(y),
               width * 3);
    }
    return buf;
}

bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida){
    QImage out(width, height, QImage::Format_RGB888);
    for (int y = 0; y < height; ++y) {
        memcpy(out.scanLine(y),
               pixelData + y * width * 3,
               width * 3);
    }
    if (!out.save(archivoSalida, "BMP")) {
        cerr << "Error: No se pudo guardar la imagen BMP modificada." << endl;
        return false;
    }
    return true;
}
