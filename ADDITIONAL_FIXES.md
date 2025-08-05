# Corrections supplémentaires pour Minishell

## Problèmes corrigés depuis la première version

### 1. **Problème de types de redirection**
**Fichiers:** `execution/redir_utils/utils.c`
**Problème:** Les types de redirection utilisaient des nombres magiques au lieu des constantes définies.
**Solution:** Utilisation des constantes `TOKEN_REDIRECT_IN`, `TOKEN_REDIRECT_OUT`, `TOKEN_APPEND` au lieu de 1, 2, 3.

### 2. **Gestion d'erreur de redirection améliorée**
**Problème:** Quand une redirection vers un fichier inexistant échouait, le programme se bloquait.
**Solution:** Amélioration de la gestion d'erreur dans `handle_input_redirect` et utilisation correcte du retour d'erreur.

## Problèmes restants à corriger

### 1. **Expansion de variables avec espaces**
**Problème:** `export t="echo hello"; $t` traite `"echo hello"` comme une seule commande au lieu de diviser en `echo` et `hello`.
**Statut:** Fonction `split_expanded_token` créée mais pas encore intégrée correctement.

### 2. **Commandes externes**
**Problème:** Les commandes comme `ls` ne fonctionnent pas correctement.
**Statut:** À investiguer.

### 3. **Heredocs**
**Problème:** Les heredocs ne sont pas implémentés.
**Statut:** À implémenter.

### 4. **Exit ne se comporte pas comme bash**
**Problème:** Exit code 2 au lieu de 0.
**Statut:** À corriger.

## Tests réussis
- ✅ Export sans double free
- ✅ Expansion simple de variables (`echo $VAR`)
- ✅ Redirections avec gestion d'erreur
- ✅ Pas de blocage sur erreurs de redirection

## Tests à faire
- 🔄 Expansion avec division (`$t` contenant plusieurs mots)
- 🔄 Commandes externes (ls, cat, etc.)
- 🔄 Heredocs
- 🔄 Exit codes corrects
