# Corrections des problèmes de double free dans Minishell

## Problèmes identifiés et corrigés

### 1. **Problème dans le garbage collector**
**Fichier:** `garbage_collector/ft_malloc.c`
**Problème:** `ft_gc_clear()` était appelé deux fois - une fois dans la boucle principale et une fois dans `ft_exit_withclear()`, causant un double free.
**Solution:** Ajout d'une vérification pour éviter le double appel :
```c
void	ft_gc_clear(void)
{
	t_list	**head;
	t_list	*current;

	head = gc_ptr();
	if (!*head)  
		return;
	current = *head;
	ft_lstclear(&current, free);
	*head = NULL;
}
```

### 2. **Problème dans `update_or_add_env`**
**Fichier:** `execution/utils2.c`
**Problème:** La fonction libérait les paramètres `name` et `value` alors qu'ils étaient utilisés par `lst_new_env` qui fait des copies.
**Solution:** Suppression des appels `free(name)` et `free(value)` dans la fonction - la libération est maintenant gérée par l'appelant.

### 3. **Problème dans `ft_export`**
**Fichier:** `execution/builtins/export.c`
**Problème:** Double libération en cas d'erreur - `validate_and_split` libère déjà en cas d'erreur, mais `ft_export` essayait aussi de libérer.
**Solution:** Gestion correcte de la libération :
```c
if (validate_and_split(args[i], &name, &value))
{
    ret = 1;
    
}
else
{
    update_or_add_env(env, name, value);
    
    if (name)
        free(name);
    if (value)
        free(value);
}
```

### 4. **Problème d'allocation mixte dans l'expansion**
**Fichier:** `expansion/utils/expand_args.c` et `tokens/token_utils3.c`
**Problème:** Mélange entre `malloc` et `ft_malloc` causant des doubles frees :
- Les tokens étaient alloués avec `ft_malloc`
- Mais `expand_arg` utilisait aussi `ft_malloc`
- `free_tokens` essayait de libérer avec `free()`

**Solution:** 
- `expand_arg` utilise maintenant `malloc` pour être cohérent avec le reste
- `free_tokens` ne libère plus les structures de tokens (libérées par le garbage collector) mais libère toujours les `value` qui utilisent `malloc` régulier

### 5. **Problème dans `ft_exit_withclear`**
**Fichier:** `execution/exit_clear.c`
**Problème:** Ordre incorrect des opérations de nettoyage.
**Solution:** Nettoyage de l'environnement avant d'appeler le garbage collector.

## Résultats

Après ces corrections :
- ✅ `export` fonctionne sans double free
- ✅ `export VAR=value` fonctionne correctement
- ✅ L'expansion de variables (`echo $VAR`) fonctionne
- ✅ `exit` fonctionne proprement
- ✅ Valgrind ne détecte plus de double free
- ✅ Les seules "fuites" détectées sont de la bibliothèque readline (normal)

## Tests effectués

1. `export TEST=hello` - ✅ Fonctionne
2. `export` - ✅ Affiche les variables d'environnement
3. `echo $TEST` - ✅ Expansion de variables
4. `exit` - ✅ Sortie propre
5. Valgrind - ✅ Pas de double free détecté

## Recommandations pour éviter de futurs problèmes

1. **Cohérence d'allocation:** Utiliser soit `malloc`/`free` soit `ft_malloc`/garbage collector, mais pas les deux pour les mêmes données
2. **Documentation:** Documenter qui est responsable de libérer chaque pointeur
3. **Tests réguliers:** Utiliser valgrind régulièrement pendant le développement
4. **Principe de responsabilité unique:** Une fonction qui alloue devrait soit libérer, soit clairement documenter qui doit libérer
