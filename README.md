# matlabtb
Python wrapper for Piotr's Computer Vision Matlab Toolbox

imResample only.
You can use this as an example if you want to add more functions.

## Requirement
- Python
- Numpy

## How to install
    
    python setup.py install
    
## How to use
example.py:
    
    import numpy as np
    import cv2
    from matlabtb import imResample
    
    image = cv2.imread(image_path, cv2.CV_LOAD_IMAGE_COLOR)
    # dtype should be float or double
    image = image.astype(np.float32)
    sz = (207, 270)
    out = np.zeros((sz[0], sz[1], img.shape[2]), dtype=img.dtype)
    
    imResample(img, out)
    
    cv2.imshow('Image', out.astype(np.uint8))
    cv2.waitKey(0)

## References
- [Piotr's Computer Vision Matlab Toolbox](https://github.com/pdollar/toolbox)
