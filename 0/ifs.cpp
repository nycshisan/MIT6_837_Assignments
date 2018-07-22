#include "ifs.h"

#include <random>

void IFS::readFromFile(const std::string &filename) {
    // open the file
    FILE *input = fopen(filename.c_str(),"r");
    assert(input != NULL);

    // read the number of transforms
    int num_transforms; 
    fscanf(input,"%d",&num_transforms);

    _nTrans = num_transforms;

    // read in the transforms
    for (int i = 0; i < num_transforms; i++) {
        float probability; 
        fscanf (input,"%f",&probability);
        Matrix m; 
        m.Read3x3(input);
        _probs.emplace_back(probability);
        _matrices.emplace_back(m);
    }

    // close the file
    fclose(input);
}

void IFS::renderToImage(Image &img, int nPoint, int nIteration) {
    Vec3f whiteColor(1.f, 1.f, 1.f), blackColor(0.f, 0.f, 0.f);
    img.SetAllPixels(whiteColor);

    for (int i = 0; i < nPoint; ++i) {
        double x = drand48(), y = drand48();
        Vec3f pVec(x, y, 1);
        for (int j = 0; j < nIteration; ++j) {
            double transChooser = drand48();
            Matrix &tran = _chooseTrans(transChooser);
            pVec = tran * pVec;
        }
        x = pVec.x() / pVec.z() * img.Width();
        y = pVec.y() / pVec.z() * img.Height();
        img.SetPixel(x, y, blackColor);
    }
}

Matrix& IFS::_chooseTrans(double transChooser) {
    int index = 0;
    float threshold = 0.0;
    while (true) {
        float newThreshold = threshold + _probs[index];
        if (newThreshold > transChooser)
            break;
        threshold = newThreshold;
        ++index;
    }
    assert(index < _matrices.size());
    return _matrices[index];
}