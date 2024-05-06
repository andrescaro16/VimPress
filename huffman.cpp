#include "huffman.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

std::unordered_map<char, unsigned int> HuffmanCoding::makeFrequencyDict(std::string text) {
    /* """Esta funcion recibe como parametro un texto plano.
				Retorna un diccionario donde el 'key' es el caracter y
				'value' equivale a las repeticiones de ese caracter
		""" */
    std::unordered_map<char, unsigned int> frequency;
    for (char character : text) {
        frequency[character]++;
    }
    return frequency;
}

void HuffmanCoding::makeHeap(const std::unordered_map<char, unsigned int>& frequency) {
    /*
    Se construye una priority queue, minHeap
				Como parametro recibe el diccionario de frecuencias
		
    */ 
    for (const auto& pair : frequency) {
        HeapNode* node = new HeapNode;
        node->data = pair.first;
        node->freq = pair.second;
        node->left = nullptr;
        node->right = nullptr;
        heap.push(node);
    }
}

void HuffmanCoding::mergeNodes() {
    /*
      Construimos el arbol de huffman.
    */ 
		
    while (heap.size() > 1) {
        HeapNode* left = heap.top();
        heap.pop();
        HeapNode* right = heap.top();
        heap.pop();

        HeapNode* merged = new HeapNode;
        merged->data = '\0';
        merged->freq = left->freq + right->freq;
        merged->left = left;
        merged->right = right;

        heap.push(merged);
    }
}

void HuffmanCoding::makeCodesHelper(HeapNode* root, std::string currentCode) {
  /*
  Recorremos el arbol de huffman para asignar el codigo binario a cada caracter
			Parametros:
			'root' -> es la raiz del arbol de huffman
			'current_code' -> Recibe un string vacio. Es el codigo binario q se va formando recursivamente. 
  */
	
    if (root == nullptr) {
        return;
    }
    if (root->data != '\0') {
        codes[root->data] = currentCode;
        reverseMapping[currentCode] = root->data;
        return;
    }
    makeCodesHelper(root->left, currentCode + "0");
    makeCodesHelper(root->right, currentCode + "1");
}

void HuffmanCoding::makeCodes() {
  /*
  Esta funcion utiliza como funcion auxiliar a make_codes_helper 
  */
	
    HeapNode* root = buildHuffmanTree();
    makeCodesHelper(root, "");
    delete root;
}

std::string HuffmanCoding::getEncodedText(const std::string& text) {
  /*
  Convierte el texto en codigo binario
			Parametros:
			'text' -> recibe un string que equivale al archivo a comprimir
			
			Retornamos el texto codificado
  */
	
    std::string encodedText;
    for (char character : text) {
        encodedText += codes[character];
    }
    return encodedText;
}

std::string HuffmanCoding::padEncodedText(const std::string& encodedText) {
  /*
  Esta funcion se utiliza para hacer que el codigo binario sea multiplo de 8, para asi guardarlo luego como bytes.
			En caso de no ser multiplo de 8 le agregamos '0' al final para forzarlo a ser multiplo de 8.
			Parametros:
				'encoded_text' -> Recibe un string del texto en codigo binario
  */
  
    unsigned int extraPadding = 8 - encodedText.size() % 8;
    std::string paddedEncodedText = std::bitset<8>(extraPadding).to_string() + encodedText;
    while (paddedEncodedText.size() % 8 != 0) {
        paddedEncodedText += "0";
    }
    return paddedEncodedText;
}

std::vector<unsigned char> HuffmanCoding::getByteArray(const std::string& paddedEncodedText) {
  /*
  Parametros:
				'padded_encoded_text' -> Recibe un string del texto en codigo binario ya equilibrado en multiplos de 8
			
			En esta funcion convertimos los bits del string en bytes y lo guardamos, esto con el fin de guardar el tamaño.

			Retorna:
				Retornamos el array de bytes con la informacion comprimida
  */
  
    std::vector<unsigned char> byteArray;
    for (size_t i = 0; i < paddedEncodedText.size(); i += 8) {
        std::string byteString = paddedEncodedText.substr(i, 8);
        byteArray.push_back(std::stoi(byteString, nullptr, 2));
    }
    return byteArray;
}

std::string HuffmanCoding::removePadding(const std::string& paddedEncodedText) {
    unsigned int extraPadding = std::stoi(paddedEncodedText.substr(0, 8), nullptr, 2);
    return paddedEncodedText.substr(8, paddedEncodedText.size() - extraPadding - 8);
}

std::string HuffmanCoding::decodeText(const std::string& encodedText) {
    std::string currentCode;
    std::string decodedText;
    for (char bit : encodedText) {
        currentCode += bit;
        if (reverseMapping.find(currentCode) != reverseMapping.end()) {
            decodedText += reverseMapping[currentCode];
            currentCode = "";
        }
    }
    return decodedText;
}

HuffmanCoding::HeapNode* HuffmanCoding::buildHuffmanTree() {
    makeHeap(makeFrequencyDict(text));
    mergeNodes();
    HeapNode* root = heap.top();
    heap.pop();
    return root;
}

std::vector<unsigned char> HuffmanCoding::compress(const std::string& text) {
    reverseMapping.clear();
    codes.clear();
    this->text = text;
    makeCodes();
    std::string encodedText = getEncodedText(text);
    std::string paddedEncodedText = padEncodedText(encodedText);
    std::vector<unsigned char> byteArray = getByteArray(paddedEncodedText);
    // Convertir reverseMapping a una cadena de caracteres
    std::string mappingString;
    for (const auto& entry : reverseMapping) {
        mappingString += entry.first + ":" + entry.second + ";"; // Ejemplo de formato simple: "key:value;"
    }
    std::cout << HEADER_END << std::endl;
    mappingString += HEADER_END;

    // Agregar la cadena de reverseMapping al principio de los datos comprimidos
    std::vector<unsigned char> finalCompressedData(mappingString.begin(), mappingString.end());
    finalCompressedData.insert(finalCompressedData.end(), byteArray.begin(), byteArray.end());

    return finalCompressedData;

}

void HuffmanCoding::reconstructReverseMapping(const std::string& mappingString) {
    std::unordered_map<std::string, char> reverseMappingHelper;

    // Variables para almacenar las partes clave y valor de cada entrada
    std::string key;
    char value;

    // Variables para analizar la cadena
    std::string entry;
    std::istringstream iss(mappingString);

    // Leer cada entrada separada por ';'
    while (std::getline(iss, entry, ';')) {
        // Separar la entrada en clave y valor usando ':'
        std::istringstream entryStream(entry);
        std::getline(entryStream, key, ':');
        entryStream >> value;
        // Agregar la entrada al mapa
        reverseMappingHelper[key] = value;
        value = ' ';
    }

    reverseMapping = reverseMappingHelper;
}


std::string HuffmanCoding::decompress(const std::vector<unsigned char>& compressedData) {

    auto it = std::search(compressedData.begin(), compressedData.end(), HEADER_END.begin(), HEADER_END.end());
    if (it == compressedData.end()) {
        // Error: No se encontró el marcador %HEADER_END%
        return "";
    }

    std::string mappingString(compressedData.begin(), it);

    reconstructReverseMapping(mappingString);

    std::string encodedText(it + HEADER_END.size(), compressedData.end());
    std::string bitString;
    for (unsigned char byte : encodedText) {
        bitString += std::bitset<8>(byte).to_string();
    }
    encodedText = removePadding(bitString);
    return decodeText(encodedText);
}
