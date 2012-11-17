% gc_chan_vese_two_stage   Calculate two-stage Chan-Vese segmentation via
%                          graph cuts.
%
% SYNOPSIS:
%  [seg energy iter1 iter2 fc1 fc2] = gc_chan_vese_two_stage(img, mu, lambda1, 
%       lambda2, bandradius, nb1, nb2, flow1, flow2, log, conv1, conv2, maxiter1, 
%       maxiter2, c1, c2)
%
% EXAMPLE:
%  a = imread('somefile.tif');
%  an = gc_normalize_image(single(a));
%  [seg energy iter1 iter2 c1 c2] = gc_chan_vese_two_stage(an, 1, 10, 10);
%
% PARAMETERS:
%  bandradius: Radius of the band that is created around the boundary of
%   the segmentation from the firstt stage. This band is then used as the
%   computational domain for the second stage.
%  See gc_chan_vese for documentation of the remaining input parameters. 
%  There are two parameters for nb, flow, conv and maxiter because different
%  values can possibly be used in the first and second stage.
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
%  mu         DOUBLE
%  lambda1    DOUBLE
%  lambda2    DOUBLE
%  bandradius DOUBLE
%  nb1        STRING
%  nb2        STRING
%  flow1      STRING
%  flow2      STRING
%  log        LOGICAL
%  conv1      DOUBLE
%  conv2      DOUBLE
%  maxiter1   DOUBLE
%  maxiter2   DOUBLE
%  c1         DOUBLE
%  c2         DOUBLE
%
%  seg        LOGICAL[]
%  energy     DOUBLE
%  iter1      DOUBLE
%  iter2      DOUBLE
%  fc1        DOUBLE
%  fc2        DOUBLE
%
% DEFAULTS:
%  img        compulsory
%  mu         compulsory
%  lambda1    compulsory
%  lambda2    compulsory
%  bandradius 8 in 2D, 4 in 3D
%  nb1        'N4' in 2D, 'N6' in 3D
%  nb2        'N16' in 2D, 'N98' in 3D
%  flow1      'GRD-KO'
%  flow2      'GRD-PRH' (is faster for dense neighbourhoods than 'GRD-KO')
%  log        false
%  conv1      0.001
%  conv2      0.001
%  maxiter1   30
%  maxiter2   10
%  c1, c2     Initialized using gc_weighted_kmeans function
%
% DESCRIPTION:
%  Computes the two-stage Chan-Vese segmentation of input image with given weights
%  via graph-cut based optimization. First a Chan-Vese segmentation is
%  computed on the whole image using a very small neighbourhood nb1 to obtain a
%  coarse segmentation. Then a band of given size is created around the
%  boundary of the segmentation from the first stage and in this band the
%  segmentation is refined using a much denser neighbourhood nb2. This approach
%  is useful with large images for which the large neighbourhood can not be
%  applied on the whole image due to memory consumption constraints and the
%  small neighbourhoods produce unsatisfactory results.
%
% NOTES:
%  See gc_chan_vese.
%
% LITERATURE:
%  See gc_chan_vese.
%
% SEE ALSO:
%  gc_normalize_image, gc_weighted_kmeans, gc_chan_vese
%
% Copyright (c) 2010 Centre for Biomedical Image Analysis
% Copyright (c) 2010 Ondrej Danek

function [seg energy iter1 iter2 fc1 fc2] = gc_chan_vese_two_stage(img, mu, lambda1, lambda2, bandradius, nb1, nb2, flow1, flow2, log, conv1, conv2, maxiter1, maxiter2, c1, c2)

% Check input data type
if (isfloat(img) == false)
    error('Use only with SINGLE[] or DOUBLE[] images.');
end

% Check parameter count
if (nargin < 4)
    error('At least 4 parameters required.');
end

if (nargin == 15)
    error('c1 and c2 have to be specified either both or none.');
end

% Supply default parameters if necessary
if (nargin < 5)
    % Default bandradius
    if (ndims(img) == 2)    
        bandradius = 8;
    else
        bandradius = 4;
    end
end

if (nargin < 6)
    % Default first-stage neighbourhood size
    if (ndims(img) == 2)
        nb1 = 'N4';
    else
        nb1 = 'N6';
    end
end

if (nargin < 7)
    % Default second-stage neighbourhood size
    if (ndims(img) == 2)
        nb2 = 'N16';
    else
        nb2 = 'N98';
    end
end

if (nargin < 8)
    % Default first-stage maximum flow algorithm
    flow1 = 'GRD-KO';
end

if (nargin < 9)
    % Default second-stage maximum flow algorithm
    flow2 = 'GRD-PRH';
end

if (nargin < 10)
    % Disable logging by default
    log = false;    
end

if (nargin < 11)
    % Default first-stage convergence criterion
    conv1 = 0.001;    
end

if (nargin < 12)
    % Default second-stage convergence criterion
    conv2 = 0.001;    
end

if (nargin < 13)
    % Default maximum number of iterations in the first stage
    maxiter1 = 30;
end

if (nargin < 14)
    % Default maximum number of iterations in the second stage
    maxiter2 = 10;
end

if (nargin < 16)
    % Init c1 and c2 using weighted kmeans algorithm
    [c1 c2] = gc_weighted_kmeans(img, lambda1, lambda2, 0.001, 30);
end

% Coarse segmentation
[seg energy iter1 c1 c2] = GcChanVese(img, lambda1 / mu, lambda2 / mu, c1, c2, conv1, maxiter1, nb1, flow1, log);

% Create band mask
mask = uint8(3) * ones(size(seg), 'uint8'); 

dt = GcDistTransform(seg, false, 'cityblock');
mask(dt > bandradius) = uint8(2); % Foreground (H(T)) constraint

dt = GcDistTransform(seg, true, 'cityblock');
mask(dt > bandradius) = uint8(1); % Background (H(S)) constraint

% Refine segmentation
[seg energy iter2 fc1 fc2] = GcChanVese(img, lambda1 / mu, lambda2 / mu, c1, c2, conv2, maxiter2, nb2, flow2, log, mask);

end