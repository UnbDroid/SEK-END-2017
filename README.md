# SEK-END-2017

 Codigo do robô para a categoria SEK da equipe END em 2017

## Nas ultimas modificações, fez-se a criação de 3 pastas:

### Funcoes_testadas
	
	São todas as funcoes que são interessantes para os codigos principais e dificilmente serão mexidas.
	A inclusão das funções serão feitas através de bibliotecas do tipo ```nome.h``` para a utilizacao nos codigos principais.

### Funcoes_nao_testadas
	
	São as funções e códigos que ainda não estão prontos e estão sendo testadas

### Codigo_principal

	São os codigos os quais coloca-se diretamente no robô. Nestes codigos utilizaremos as funcoes já testadas da pasta ```Funcoes_testadas```.


## Como utilizar os codigos

	O NXC não aceita modularização e não há possibilidade de criar bibliotecas. Assim, para melhorar a utilização do código(e leitura), fez-se um esquema para montar o arquivo ```nome.nxc``` a partir do arquivo ```nome.c```, visto que C aceita a inclusão de bibliotecas além de no sublime já ter a sintaxe bem definida para C. 
	Foi feito um arquivo chamado ```executar.py```. Este programa em python analisa um codigo principal(vamos chamar por exemplo de ```base.c```) e cria um arquivo ```base.nxc```. 

### O funcionamento do programa ```executar.py```

	Em resumo, esse programa copia o conteudo de ```nome.c``` e grava em ```nome.nxc```, mas no lugar dos includes ele substitui pelo arquivo do include. Por exemplo, se o programa ```nome.c``` existe o include ```a.h``` e ```b.h```, então no lugar do comando ```#include "a.h"```, coloca-se todo o conteudo do arquivo ```a.h``` e de mesma maneira, no lugar de ```#include "b.h"```, o  conteudo do arquivo ```b.h```. Isso é feito de maneira recursiva.

	Uma explicação mais detalhada é mostrada abaixo:

	O programa inicia uma lista vazia chamada ```defines```. O programa então procura linha a linha a palavra ```#define``` e caso ache, adiciona na lista ```defines```. Uma vez que acha um ```#include```, ele procura todos os ```#define``` do arquivo incluido e adiciona na lista ```defines```. Isso é feito recursivamente.
	Assim, tem-se todos os defines de todos os arquivos inclusos. Após isso, escreve-se todos os defines em ```nome.nxc```.
	Então, depois de escrever os defines, o programa copia e cola todas as funções dos arquivos incluidos na ordem que são declarados.

	Para mais detalhes, leia o algoritmo.

### Como compilar o NXC

	Uma vez que temos o programa ```executar.py```, precisamos definir qual o arquivo(o ```nome.c```) que deve ser utilizado para ser transformado para ```nome.nxc```. Dentro do programa ```executar.py``` há duas linhas: uma comentadada e outra não. Assim, se a linha:

	```
	arquivo = "Codigo_principal/Teste_Caminhos"
	```

	estiver ativa, então será utilizado o arquivo ```Teste_Caminhos.c``` da pasta ```Codigo_principal``` e será transformado para ```Teste_Caminhos.ncx```.

	A partir daí, se executar o programa ```executar.py```, será criado o arquivo ```Teste_Caminhos.nxc``` na mesma pasta do programa ```executar.py```. Uma vez que se tem o ```.nxc``` para compilar e colocar dentro do cérebro, fica a critério de quem programa a maneira que compila e passa ao cérebro


	Lembre-se que como o ```.nxc``` é gerado a partir do ```.c```, então não é necessário colocar o ```.nxc``` no ```git```. Atualize apenas o arquivo ```.c```.

## Comentarios adicionais

###	Gravação de arquivos no cérebro

	Existe um codigo para gravação de arquivos no cérebro que está na pasta ```Funcoes_testadas``` com o nome de ```arquivo.nxc```. Dê uma olhada

### Contato

	Duvidas, sugestões, críticas são bem vindas. Os e-mails para contato dos contribuidores desse respositório são:

	Carlos Adir - ```carlos.adir.leite@gmail.com```
	
	Kaynan Rodrigues - ```kaynarodrigues@live.com```

	Rodrigo - ```drigo.z.m@hotmail.com```

	Vieira - ```vieira.arimatea@gmail.com```