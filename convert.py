import os
import glob
import numpy as np
from PIL import Image


width = 256
height = 256


raw_files = glob.glob("*.raw")

if not raw_files:
    print("No .raw files found to convert!")

for raw_file in raw_files:
    png_file = raw_file.replace(".raw", ".png")
    try:
        
        data = np.fromfile(raw_file, dtype=np.uint8)
      
        image_array = data.reshape((height, width))
     
        img = Image.fromarray(image_array, mode='L')
        img.save(png_file)
        print(f"✅ Converted: {raw_file} -> {png_file}")
    except Exception as e:
        print(f"❌ Error converting {raw_file}: {e}")