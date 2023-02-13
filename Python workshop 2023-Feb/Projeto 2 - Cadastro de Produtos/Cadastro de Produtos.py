#Cliente precisa que 500 produtos sejam cadastrados no meu sistema online, com base em uma planilha que sera fornecida.

#https://cadastroprodutos-devaprender.netlify.app/

#pip install pyautogui
#pip install pyllow
#pip install mouseinfo
#pip install openpyxl

#>>> from mouseinfo import mouseInfo
#>>> mouseInfo()

#A planilha pode ser baixada em: 
#https://drive.google.com/drive/folders/1WstT4hIDVDTa06HfLBTPZrzuCbtQjqoC

#Abrir o site
#Abrir a planilha
#Copiar dado prim. col.
#colar no site
#repeat untill EOS

import pyautogui
import pyperclip
import openpyxl
import os

absolute_path = os.path.dirname(__file__)
relative_path = "Produtos.xlsx"
full_path = os.path.join(absolute_path, relative_path)

#abrir a planilha
workbook = openpyxl.load_workbook(full_path)
sheet_produtos = workbook['produtos']
for linha in sheet_produtos.iter_rows(min_row=2, max_row=501):
    produto = linha[0].value
    fornecedor = linha[1].value
    categoria =  linha[2].value
    quantidade = linha[3].value
    valor_unit = linha[4].value
    notificar_venda = linha[5].value
    pyautogui.click(-590,398,duration=.1)
    pyautogui.typewrite(produto)
    pyautogui.click(-590,493,duration=.1)
    pyautogui.typewrite(fornecedor)
    pyautogui.click(-590,587,duration=.1)
    pyperclip.copy(categoria)
    pyautogui.hotkey('ctrl','v')
    pyautogui.click(-590,683,duration=.1)
    pyautogui.typewrite(valor_unit)
    if notificar_venda == "Sim":
        pyautogui.click(-600,770,duration=.1)
    elif notificar_venda == "Não":
        pyautogui.click(-494,770,duration=.1)
    pyautogui.click(-500,840,duration=.1)
        