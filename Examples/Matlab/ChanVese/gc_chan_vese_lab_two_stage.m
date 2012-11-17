% gc_chan_vese_lab_two_stage   Calculate two-stage label preserving Chan-Vese 
%                              segmentation via graph cuts.
%
% SYNOPSIS:
%  [seg energy iter1 iter2 fc1 fc2] = gc_chan_vese_lab_two_stage(img, ilab, mu, 
%       lambda1, lambda2, bandradius, nb1, nb2, log, conv1, conv2, maxiter1, 
%       maxiter2, c1, c2)
%
% EXAMPLE: (use segmentation from one image as initialization on another one)
%  s1 = imread('slice1.tif');
%  s2 = imread('slice2.tif');
%  s1n = gc_normalize_image(single(s1));
%  s2n = gc_normalize_image(single(s2));
%  seg1 = uint8(bwlabel(gc_chan_vese(s1n, 1, 10, 10)));
%  [seg2 energy iter c1 c2] = gc_chan_vese_lab_two_stage(s2n, seg1, 1, 10, 10);
%
% PARAMETERS:
%  See gc_chan_vese_lab and gc_chan_vese_two_stage for documentation of the 
%  input parameters.
%
%  seg: Segmentation mask.
%  energy: Energy of the final segmentation.
%  iter1: Number of iterations performed in the first stage.
%  iter2: Number of iterations performed in the second stage.
%  fc1: Final background mean intensity.
%  fc2: Final foreground mean intensity.
%
% PARAMETERS:
%  img        SINGLE[] or DOUBLE[]
%  ilab       UINT8[]
%  mu         DOUBLE
%  lambda1    DOUBLE
%  lambda2    DOUBLE
%  bandradius DOUBLE
%  nb1        STRING
%  nb2        STRING
%  log        LOGICAL
%  conv1      DOUBLE
%  conv2      DOUBLE
%  maxiter1   DOUBLE
%  maxiter2   DOUBLE
%  c1         DOUBLE
%  c2         DOUBLE
%
%  seg        UINT8[]
%  energy     DOUBLE
%  iter1      DOUBLE
%  iter2      DOUBLE
%  fc1        DOUBLE
%  fc2        DOUBLE
%
% DEFAULTS:
%  img        compulsory
%  ilab       compulsory
%  mu         compulsory
%  lambda1    compulsory
%  lambda2    compulsory
%  bandradius 8 in 2D, 4 in 3D
%  nb1        'N4' in 2D, 'N6' in 3D
%  nb2        'N16' in 2D, 'N98' in 3D
%  log        false
%  conv1      0.001
%  conv2      0.001
%  maxiter1   30
%  maxiter2   10
%  c1, c2     Initialized using gc_weighted_kmeans function
%
% DESCRIPTION:
%  Computes the two-stage label preserving Chan-Vese segmentation of the
%  input image with given weights via graph-cut based optimization. See
%  gc_chan_vese_lab and gc_chan_vese_two_stage for more details.
%
% NOTES:
%  See gc_chan_vese.
%
% LITERATURE:
%  See gc_chan_vese_lab.
%
% SEE ALSO:
%  gc_normalize_image, gc_weighted_kmeans, gc_chan_vese, gc_chan_vese_two_stage,
%  gc_chan_vese_lab
%
% Copyright (c) 2010 Centre for Biomedical Image Analysis
% Copyright (c) 2010 Ondrej Danek

function [seg energy iter1 iter2 fc1 fc2] = gc_chan_vese_lab_two_stage(img, ilab, mu, lambda1, lambda2, bandradius, nb1, nb2, log, conv1, conv2, maxiter1, maxiter2, c1, c2)

% Check input data type
if (isfloat(img) == false)
    error('Use only with SINGLE[] or DOUBLE[] images.');
end

% Check parameter count
if (nargin < 5)
    error('At least 5 parameters required.');
end

if (nargin == 14)
    error('c1 and c2 have to be specified either both or none.');
end

% Supply default parameters if necessary
if (nargin < 6)
    % Default bandradius
    if (ndims(img) == 2)    
        bandradius = 8;
    else
        bandradius = 4;
    end
end

if (nargin < 7)
    % Default first-stage neighbourhood size
    if (ndims(img) == 2)
        nb1 = 'N4';
    else
        nb1 = 'N6';
    end
end

if (nargin < 8)
    % Default second-stage neighbourhood size
    if (ndims(img) == 2)
        nb2 = 'N16';
    else
        nb2 = 'N98';
    end
end

if (nargin < 9)
    % Disable logging by default
    log = false;    
end

if (nargin < 10)
    % Default first-stage convergence criterion
    conv1 = 0.001;    
end

if (nargin < 11)
    % Default second-stage convergence criterion
    conv2 = 0.001;    
end

if (nargin < 12)
    % Default maximum number of iterations in the first stage
    maxiter1 = 30;
end

if (nargin < 13)
    % Default maximum number of iterations in the second stage
    maxiter2 = 10;
end

if (nargin < 14)
    % Init c1 and c2 using weighted kmeans algorithm
    [c1 c2] = gc_weighted_kmeans(img, lambda1, lambda2, 0.001, 30);
end

% Coarse segmentation
[seg energy iter1 c1 c2] = GcChanVeseLab(img, ilab, lambda1 / mu, lambda2 / mu, c1, c2, conv1, maxiter1, nb1, log);

% Create band mask
mask = uint8(3) * ones(size(seg), 'uint8'); 

dt = GcDistTransform(seg > 0, false, 'cityblock');
mask(dt > bandradius) = uint8(2); % Foreground (H(T)) constraint

dt = GcDistTransform(seg > 0, true, 'cityblock');
mask(dt > bandradius) = uint8(1); % Background (H(S)) constraint

% Refine segmentation
[seg energy iter2 fc1 fc2] = GcChanVeseLab(img, seg, lambda1 / mu, lambda2 / mu, c1, c2, conv2, maxiter2, nb2, log, mask);

end