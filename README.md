# 數位影像處理概論

### 影像處理程式開發平台3

**1.問題描述**
    因為疫情的原因，口罩是每個人的出門必須帶的必需品，剛好影像處理，讓人馬上聯想到可以做，偵測及辨識等...，
  所以我想做口罩偵測，判斷這個人是否有帶口罩。
  
 **2.預計使用的技術與理論**
     先使用OpenCV，去讀別人做好的人臉偵測(.xml)，再去研究如何自己訓練出口罩偵測，來實現判別是否有帶口罩。


---

**預計新增功能**

- 人臉偵測 : 先使用別人訓練好的(.xml)模組。
- 口罩偵測 : 尋找如何自己訓練(.xml)模組訓練。

---

**新增功能以下功能:**

- 找尋輪廓

- 邊緣與輪廓

- 膨脹(dilation)與侵蝕(erosion)


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
