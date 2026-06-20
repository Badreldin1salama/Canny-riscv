clc; clear; close all;

% 1. اقرأ الصورة الأصلية
img = imread('input.png'); % حط اسم صورتك هنا

% 2. إجبار الصورة تكون 1 Channel (Grayscale)
if size(img, 3) > 1
    img = rgb2gray(img);
end

% 3. تغيير الحجم لـ 256x256 بالظبط
width = 256;
height = 256;
img = imresize(img, [height width]);

% 4. التريكة الهندسية: عكس المصفوفة (Transpose) عشان الـ C++ يقراها صفوف مش أعمدة
img_transposed = img';

% 5. كتابة البايتات في ملف Raw
fileID = fopen('test_1.raw', 'w');
fwrite(fileID, img_transposed, 'uint8');
fclose(fileID);

disp('تم إنشاء ملف test_1.raw بنجاح! جاهز للـ C++');