// ldd: -lncurses
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

int plateau[4][4];
int points = 0;

void miroir_lignes()
{
    for (int i = 0; i < 4; i++)
    {
        int tmp;
        tmp = plateau[i][0]; plateau[i][0] = plateau[i][3]; plateau[i][3] = tmp;
        tmp = plateau[i][1]; plateau[i][1] = plateau[i][2]; plateau[i][2] = tmp;
    }
}

void symetrie_diagonale()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            int tmp = plateau[i][j];
            plateau[i][j] = plateau[j][i];
            plateau[j][i] = tmp;
        }
    }
}

int tasser()
{
    int bouge = 0;
    for (int i = 0; i < 4; i++)
    {
        int file[4] = {0, 0, 0, 0};
        int pos = 0;
        for (int j = 0; j < 4; j++)
            if (plateau[i][j] != 0)
                file[pos++] = plateau[i][j];
        for (int j = 0; j < 4; j++)
        {
            if (plateau[i][j] != file[j])
                bouge = 1;
            plateau[i][j] = file[j];
        }
    }
    return bouge;
}

int compacter_gauche()
{
    int modifie = tasser();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (plateau[i][j] != 0 && plateau[i][j] == plateau[i][j + 1])
            {
                plateau[i][j] *= 2;
                points += plateau[i][j];
                plateau[i][j + 1] = 0;
                modifie = 1;
            }
        }
    }
    tasser();
    return modifie;
}

int aller_gauche() { return compacter_gauche(); }
int aller_droite() { miroir_lignes(); int c = compacter_gauche(); miroir_lignes(); return c; }
int aller_haut()   { symetrie_diagonale(); int c = compacter_gauche(); symetrie_diagonale(); return c; }
int aller_bas()
{
    symetrie_diagonale();
    miroir_lignes();
    int c = compacter_gauche();
    miroir_lignes();
    symetrie_diagonale();
    return c;
}

int coup_possible()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (plateau[i][j] == 0) return 1;
            if (j < 3 && plateau[i][j] == plateau[i][j + 1]) return 1;
            if (i < 3 && plateau[i][j] == plateau[i + 1][j]) return 1;
        }
    return 0;
}

int cases_libres()
{
    int total = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (plateau[i][j] == 0)
                total++;
    return total;
}

void nouvelle_tuile()
{
    int libre = cases_libres();
    if (libre == 0)
        return;
    int n = rand() % libre;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (plateau[i][j] == 0)
            {
                if (n == 0)
                {
                    plateau[i][j] = 2;
                    return;
                }
                n--;
            }
        }
    }
}

void vider_plateau()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            plateau[i][j] = 0;
}

void dessiner()
{
    clear();
    printw("====== 2048 ======\n");
    printw("Score: %d\n\n", points);

    printw("+------+------+------+------+\n");
    for (int i = 0; i < 4; i++)
    {
        printw("|");
        for (int j = 0; j < 4; j++)
        {
            if (plateau[i][j] == 0)
                printw("      |");
            else
                printw("%5d |", plateau[i][j]);
        }
        printw("\n");
        printw("+------+------+------+------+\n");
    }

    printw("\nfleches = deplacer  |  q = quitter\n");
    refresh();
}

int main()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    srand(time(NULL));

    vider_plateau();
    nouvelle_tuile();
    nouvelle_tuile();
    dessiner();

    int touche;
    while ((touche = getch()) != 'q')
    {
        int modifie = 0;
        if      (touche == KEY_LEFT)  modifie = aller_gauche();
        else if (touche == KEY_RIGHT) modifie = aller_droite();
        else if (touche == KEY_UP)    modifie = aller_haut();
        else if (touche == KEY_DOWN)  modifie = aller_bas();

        if (modifie)
            nouvelle_tuile();

        dessiner();

        if (!coup_possible())
        {
            printw("\nGAME OVER — Score final: %d\n", points);
            printw("Appuyez sur une touche...\n");
            refresh();
            getch();
            break;
        }
    }

    endwin();
    return 0;
}
