# 數位影像處理概論

### 期末影像處理程式開發平台

**1.問題描述**

   因為疫情的原因，口罩是每個人的出門必須帶的必需品，剛好影像處理，讓人馬上聯想到可以做，偵測及辨識等...，所以我想做口罩偵測，判斷這個人是否有帶口罩。
  
 **2.預計使用的技術與理論**
 
   先使用OpenCV，去讀別人做好的人臉偵測(.xml)，來實現判別是否有帶口罩。


---

**開發環境**

使用開發軟體工具：Visual Studio 2022

使用語言：C++

UI介面開發工具：Qt Designer

---

**口罩辨識**

   使用Opencv內訓練好的分類器，來偵測臉、鼻子、嘴巴，再來判斷，是否有戴口罩，是否有戴好口罩。
   
   
**執行結果**  

   ![image](https://user-images.githubusercontent.com/99851840/172576780-16b91552-dc32-4848-b731-529bc376c062.png)
   
   有戴好口罩

   ![image](https://user-images.githubusercontent.com/99851840/172576751-5053ba55-af29-4cfd-92e5-001ce73e8feb.png)
   
   沒帶口罩

   ![image](https://user-images.githubusercontent.com/99851840/172576675-1a4c2214-f026-4c56-8802-45b014f92cd9.png)
   
   戴口罩露出鼻子

---

**已經做出功能以下功能:**

- 開啟圖檔
- ROI
- 低通濾波器
  - 平滑模糊
  - 高斯模糊
  - 中值濾波器
  - 雙邊濾波器
- 高通濾波器
  - 拉普拉斯
  - Sobel
  - Canny
- 仿射轉換
  - 旋轉
  - 平移
  - 翻轉
- 透視投影
- Gray & HSV
- Binary_Value
- 自訂參數的仿射轉換
- 找尋輪廓
- 邊緣與輪廓
- 膨脹(dilation)與侵蝕(erosion)
