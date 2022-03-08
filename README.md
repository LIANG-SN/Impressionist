# Impressionist

HKUST COMP4411 Project1

W per new brush						yes/

W border clipping					yes/

W (red) marker						yes/

W swap							yes/

B RGB scale						yes/

B blur/sharp brush					yes/

B 1 level undo						yes/

B dissolve						yes/

BW fade in/out input on output canvas			yes/

BW mural (blending)					yes/

BW alpha brush						yes/

BW automatic paint whole image				yes/

BW gradient from user-specified image			yes/

BW edge clipping					yes/

2B user-specified kernel				yes/

2B edge detection + user-specified edge clipping	yes/

2B multiresolution					yes/

2B warp brush						yes/

2B curved brush following image gradient		yes/

4B mosaic						yes/

4B impressionist video with temporal coherency		yes/

8B Bayesian matting					yes/

Readme:
1. multiresolution: multiresolution can be used by derectly clicking "multiresolution" in the function menu(which is inplemented using point brush), 
		    the paintly button contains some extra feature, which combines multiresolution and curve brush.
2. user-specified kernel: enter kernel line by line and split numbers with space
3. impressionist video feature: load a video from the video directory and it will directly play it on paintview. 
   				For customization, simply change the attributes in brush.
4. matting: simplified (but not so effective) KNN. First load an image(eg. fox), then click load alpha image for matting(eg. fox_alpha)
	    Then click matting in menu bar.
