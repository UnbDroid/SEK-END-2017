'''

Como o nxc não aceita bibliotecas, esse programa copia os codigos das bibliotecas para um arquivo temporário e executa esse arquivo
temporário.

'''



def existe_arquivo(nome):
	try:
		arquivo = open(nome, "r")
		arquivo.close()
		return True
	except:
		pass
	return False

def le_arquivo(nome):
	arquivo = open(nome, "r")
	linhas = arquivo.readlines()
	arquivo.close()
	for i in range(len(linhas)):
		linhas[i] = linhas[i].split('\n')[0]
	return linhas

def leia_do_arquivo(nome):
	defines = []
	codigo  = []
	linhas = le_arquivo(nome)
	for linha in linhas:
		if "#define" in linha:
			defines.append(linha)
		elif "#include" in linha:
			novo_arquivo = linha.split('"')[1]
			new_defines, new_codigos = leia_do_arquivo(novo_arquivo)
			for defi in new_defines:
				if not defi in defines:
					defines.append(defi)
			for code in new_codigos:
				codigo.append(code)
		else:
			codigo.append(linha)
	return defines, codigo



if __name__ == "__main__":
	#arquivo = "Codigo_principal/B.O.S.T.A.nxc"
	arquivo = "Teste_Caminhos"
	defines, codigo = leia_do_arquivo(arquivo + ".c")
	new_arq = open(arquivo + ".nxc", "w")
	for defin in defines:
		new_arq.write(defin + "\n")
	new_arq.write("\n\n")
	for line in codigo:
		new_arq.write(line + "\n")
	new_arq.close()
