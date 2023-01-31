# O sistema de login deve permitir que os novos usuarios sejam cadastrados, e que usuarios existentes possam fazer o login. O sistema deve alertar caso a senha e login nao estejam corretos.

# 1 - O sistema de login deve permitir que novos usuarios sejam cadastrados
# 2 - O sistema nao deve permitir que usuarios duplicados sejam cadastrados
# 3 - Permitir que usuarios existentes possam fazer o login
# 4 - O sistema deve alertar caso a senha ou login nao estejam corretos

#Limpar terminal
import os
os.system('cls')


#lista rudimentar de usuarios
usuarios = ['carol','amanda','jeff']
senhas = ['123456','abcdef','123abc']


def cadastro_usuario():
    #Input usuario
    usuario_digitado = input('digite seu usuario: ')
    senha_digitada = input('digite sua senha: ')
    #Adicionar na lista
    if Checar_usuario(usuario_digitado) == 0:
        usuarios.append(usuario_digitado)
        senhas.append(senha_digitada)
    else:
        print('usuario ja existente')


def Checar_senha(senha_digitada):
    #checa a senha
    for indice, item in enumerate(usuarios):
        if senhas[indice] == senha_digitada:
            return 1
        else:
            return 0
    else:
        return


def Checar_usuario(usuario_digitado):
    if usuario_digitado in usuarios:
        return 1
    else:
        return 0


def Login_usuario():
    #recebe informacoes de login e senha e chama a funcao para checar
    usuario_digitado = input('digite seu usuario: ')
    senha_digitada = input('digite sua senha: ')
    if Checar_usuario(usuario_digitado) == 1: 
        if Checar_senha(senha_digitada) == 1:
            print('Login realizado')
            return 1
        else:
            print('Falha no login')
            return 0
    else:
        return 0
        
        
def Menu_inicial():
    #Menu selecao usuario
    resposta = input('[1] - Cadastrar novo usuario [2] - Fazer login: ')
    if resposta == '1' :
        cadastro_usuario()
    elif resposta =='2':
        Login_usuario()


Menu_inicial()
print(usuarios)
print(senhas)


