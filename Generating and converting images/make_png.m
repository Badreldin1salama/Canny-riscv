clc; clear; close all;

% 1. نفس الأبعاد اللي اتفقنا عليها
width = 256;
height = 256;

% 2. فتح وقراءة ملف الـ Raw اللي طلع من الكود بتاعك
fileID = fopen('input.raw', 'r');
if fileID == -1
    error('الملف مش موجود! اتأكد من المسار والاسم.');
end
% ماتلاب بيقرا الداتا في مصفوفة ثنائية الأبعاد مباشرة
raw_data = fread(fileID, [width height], 'uint8');
fclose(fileID);

% 3. نرجع نعكس المصفوفة للوضع الطبيعي بتاع الصور
img_restored = raw_data';

% 4. عرض الصورة على الشاشة
figure;
imshow(uint8(img_restored));
title('Output png Image');

% 5. (اختياري) حفظ النتيجة كصورة PNG عادية عشان لو حبيت تحطها في التقرير
imwrite(uint8(img_restored), 'final_blurred_result.png');
disp('تم عرض الصورة وحفظها كـ PNG بنجاح!');