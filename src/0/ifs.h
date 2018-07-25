#ifndef MIT6837ASSIGNMENT_0_IFS
#define MIT6837ASSIGNMENT_0_IFS

#include <string>
#include <vector>

#include "image.h"
#include "matrix.h"

class IFS {
    int _nTrans;
    std::vector<Matrix> _matrices;
    std::vector<float> _probs;

    Matrix& _chooseTrans(double transChooser);

public:
    IFS() = default;

    void readFromFile(const std::string &filename);
    void renderToImage(Image &img, int nPoint, int nIteration);
};

#endif