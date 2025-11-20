On est libre sur la syntaxe du fichier de config
Le plus simple et le plus intéressant est de suivre la syntaxe de NGINX

Définir des paires interface:port (multi ports).
Avoir un fichier de config utilisé par le programme.
Pouvoir définir des default error pages.
Pouvoir définir la taille max du body (client_max_body_size).
Pouvoir définir, par route (URL) :
les méthodes HTTP acceptées (au moins GET, POST, DELETE sont supportées globalement),
la redirection HTTP,
le dossier racine (root),
activer/désactiver le directory listing (autoindex),
les fichiers index par défaut pour un répertoire,
l’upload (autorisé + dossier de stockage),
l’exécution d’un CGI pour au moins une extension.




TDL :
[X] - Questions
[X] - Logique error_pages
[] - Faire des tests avec de fichier de config de merde
[] - meilleur organisation des fichiers
[] - comprendre les élements du fichier de config
[] - GPT comment travailler à deux sur un projet, bonne pratique
[] - faire ma branche sur le git de nils
[] - lui dire de mettre tout sur le main
[] - mettre sur le main et l'informé