# SISTEMA DE CADASTRO DE ALUNOS E NOTAS
Sistema desenvolvido em Python para cadastrar, consultar e excluir alunos e suas notas de forma dinâmica.

## Fluxo de Dados Básico
Usuário escolhe uma opção no menu
        │
        ├── [1] Cadastrar
        │       │
        │       ├── Informa o nome do aluno
        │       ├── Informa quantas notas deseja cadastrar
        │       ├── Digita cada nota (validada entre 0 e 10)
        │       └── Dados salvos em uma lista
        │
        ├── [2] Consultar
        │       │
        │       ├── Informa o nome do aluno
        │       └── Exibe nome e todas as notas cadastradas
        │
        ├── [3] Excluir
        │       │
        │       ├── Informa o nome do aluno
        │       └── Remove o aluno do sistema
        │
        └── [4] Sair

## Conceitos Utilizados

- Funções
- Listas e manipulação de índices
- Match/Case
- While e For
- Try/Except para validação de entradas
