% gc_chan_vese_lab   Calculate graph cut based Chan-Vese segmentation
%                    integrated with object indication function.
%                    
%
% SYNOPSIS:
%  [seg energy iter fc1 fc2] = gc_chan_vese_lab(img, ilab, mu, lambda1,
%       lambda2, mask, nb, log, conv, maxiter, c1, c2)
%
% EXAMPLE: (use segmentation from one image as initialization on another one)
%  s1 = imread('slice1.tif');
%  s2 = imread('slice2.tif');
%  s1n = gc_normalize_image(single(s1));
%  s2n = gc_normalize_image(single(s2));
%  seg1 = uint8(bwlabel(gc_chan_vese(s1n, 1, 10, 10)));
%  [seg2 energy iter c1 c2] = gc_chan_vese_lab(s2n, seg1, 1, 10, 10);
%
% PARAMETERS:
%  ilab: Initial object indicators.
%  See gc_chan_vese for the documentation of the remaining input parameters.
%
%  seg: Final segmentation (object indicators).
%  energy: Energy of the solution.
%  iter: Number of iterations performed.
%  fc1: Final background mean intensity.
%  fc2: Final foreground mean intensity.
%
% PARAMETERS:
%  img        SINGLE[] or DOUBLE[]
%  ilab       UINT8[]
%  mu         DOUBLE
%  lambda1    DOUBLE
%  lambda2    DOUBLE
%  mask       UINT8[]
%  nb         STRING
%  log        LOGICAL
%  conv       DOUBLE
%  maxiter    DOUBLE
%  c1         DOUBLE
%  c2         DOUBLE
%
%  seg        UINT8[]
%  energy     DOUBLE
%  iter       DOUBLE
%  fc1        DOUBLE
%  fc2        DOUBLE
%
% DEFAULTS:
%  img        compulsory
%  ilab       compulsory
%  mu         compulsory
%  lambda1    compulsory
%  lambda2    compulsory
%  mask       []
%  nb         'N16' in 2D, 'N26' in 3D
%  log        false
%  conv       0.001
%  maxiter    30
%  c1, c2     Initialized using gc_weighted_kmeans function
%
% DESCRIPTION:
%  Computes the Chan-Vese segmentation of the input image integrated with
%  object indication function via graph-cut based optimization. TODO.
%
% NOTES:
%  See gc_chan_vese.
%
% LITERATURE:
%  [1] Not yet published. TODO.
%  [*] See gc_chan_vese for more.
%
% SEE ALSO:
%  gc_normalize_image, gc_weighted_kmeans, gc_chan_vese, gc_chan_vese_two_stage, 
%  gc_chan_vese_lab_two_stage
%
% Copyright (c) 2010 Centre for Biomedical Image Analysis
% Copyright (c) 2010 Ondrej Danek

function [seg energy iter fc1 fc2] = gc_chan_vese_lab(img, ilab, mu, lambda1, lambda2, mask, nb, log, conv, maxiter, c1, c2)

% Check input data type
if (isfloat(img) == false)
    error('Use only with SINGLE[] or DOUBLE[] images.');
end

% Check parameter count
if (nargin < 5)
    error('At least 5 parameters required.');
end

if (nargin == 11)
    error('c1 and c2 have to be specified either both or none.');
end

% Supply default parameters if necessary
if (nargin < 7)
    % Default neighbourhood size
    if (ndims(img) == 2)
        nb = 'N16';
    else
        nb = 'N26';
    end
end

if (nargin < 8)
    % Disable logging by default
    log = false;    
end

if (nargin < 9)
    % Default convergence criterion
    conv = 0.001;    
end

if (nargin < 10)
    % Default maximum number of iterations
    maxiter = 30;
end

if (nargin < 12)
    % Init c1 and c2 using weighted kmeans algorithm
    [c1 c2] = gc_weighted_kmeans(img, lambda1, lambda2, 0.001, 30);
end

% Compute label preserving segmentation
if (nargin == 5 || numel(mask) == 0)
    % Compute unmasked segmentation
    [seg energy iter fc1 fc2] = GcChanVeseLab(img, ilab, lambda1 / mu, lambda2 / mu, c1, c2, conv, maxiter, nb, log);
else
    % Compute masked segmentation
    [seg energy iter fc1 fc2] = GcChanVeseLab(img, ilab, lambda1 / mu, lambda2 / mu, c1, c2, conv, maxiter, nb, log, mask);
end

end