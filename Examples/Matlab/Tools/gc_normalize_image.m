% gc_normalize_image   Normalize image intensities.
%
% SYNOPSIS:
%  image_out = gc_normalize_image(image_in)
%
% EXAMPLE:
%  a = imread('somefile.tif');
%  an = gc_normalize_image(single(a));
%
% PARAMETERS:
%  image_in: Input image.
%
% PARAMETERS:
%  image_in   SINGLE[] or DOUBLE[]
%
% DESCRIPTION:
%  Fit image intensities to [0,1] interval. 
%
% Copyright (c) 2010 Centre for Biomedical Image Analysis
% Copyright (c) 2010 Ondrej Danek

function nimg = gc_normalize_image(img)

if (isfloat(img) == false)
    error('Use only with SINGLE[] or DOUBLE[] images.');
end

a = min(img(:));
b = max(img(:));

nimg = (img - a) / (b - a);

end

