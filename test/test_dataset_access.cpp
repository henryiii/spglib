#include <gtest/gtest.h>

extern "C" {
#include "spglib.h"
#include "utils.h"
}

static void show_spacegroup_type(const SpglibSpacegroupType spgtype);

TEST(test_dataset_access, test_spg_get_symmetry_from_database) {
    int rotations[192][3][3];
    double translations[192][3];
    int i, j, size;

    size = spg_get_symmetry_from_database(rotations, translations, 460);
    ASSERT_EQ(size, 36);
    show_symmetry_operations(rotations, translations, size);
}

TEST(test_dataset_access, test_spg_get_spacegroup_type) {
    SpglibSpacegroupType spgtype;
    spgtype = spg_get_spacegroup_type(446);
    ASSERT_EQ(spgtype.number, 156);
}

TEST(test_dataset_access, test_spg_get_hall_number_from_symmetry) {
    /* Im-3m (229) */
    double lattice[3][3] = {{4, 0, 0}, {0, 4, 0}, {0, 0, 4}};
    double position[][3] = {{0, 0, 0}, {0.5, 0.5, 0.5}};
    int types[] = {1, 1};
    int num_atom = 2;
    int retval = 0;
    double symprec = 1e-5;

    int hall_number;
    SpglibSpacegroupType spgtype;
    SpglibDataset *dataset;

    dataset = NULL;

    dataset = spg_get_dataset(lattice, position, types, num_atom, symprec);
    ASSERT_TRUE(dataset != NULL);

    printf("hall_number = %d is found by spg_get_dataset.\n",
           dataset->hall_number);
    hall_number = spg_get_hall_number_from_symmetry(
        dataset->rotations, dataset->translations, dataset->n_operations,
        symprec);
    printf("hall_number = %d is found by spg_get_hall_number_from_symmetry.\n",
           hall_number);
    ASSERT_EQ(hall_number, dataset->hall_number);
    ASSERT_EQ(hall_number, 529);

    spgtype = spg_get_spacegroup_type(hall_number);
    ASSERT_EQ(spgtype.number, 229);
    show_spacegroup_type(spgtype);

    if (dataset) {
        spg_free_dataset(dataset);
    }
}

TEST(test_dataset_access, test_spg_get_spacegroup_type_from_symmetry) {
    /* Im-3m (229) */
    double lattice[3][3] = {{4, 0, 0}, {0, 4, 0}, {0, 0, 4}};
    double position[][3] = {{0, 0, 0}, {0.5, 0.5, 0.5}};
    int types[] = {1, 1};
    int num_atom = 2;
    int retval = 0;
    double symprec = 1e-5;

    int hall_number;
    SpglibSpacegroupType spgtype;
    SpglibDataset *dataset;

    dataset = NULL;

    dataset = spg_get_dataset(lattice, position, types, num_atom, symprec);
    ASSERT_TRUE(dataset != NULL);

    printf("hall_number = %d is found by spg_get_dataset.\n",
           dataset->hall_number);
    spgtype = spg_get_spacegroup_type_from_symmetry(
        dataset->rotations, dataset->translations, dataset->n_operations,
        lattice, symprec);
    printf("number = %d is found by spg_get_spacegroup_type_from_symmetry.\n",
           spgtype.number);

    show_spacegroup_type(spgtype);
    ASSERT_EQ(spgtype.number, dataset->spacegroup_number);
    ASSERT_EQ(spgtype.number, 229);

    if (dataset) {
        spg_free_dataset(dataset);
    }
}

// ****************************************************************************
// Local functions
// ****************************************************************************

static void show_spacegroup_type(const SpglibSpacegroupType spgtype) {
    printf("Number:            %d\n", spgtype.number);
    printf("International:     %s\n", spgtype.international_short);
    printf("International:     %s\n", spgtype.international_full);
    printf("International:     %s\n", spgtype.international);
    printf("Schoenflies:       %s\n", spgtype.schoenflies);
    printf("Hall symbol:       %s\n", spgtype.hall_symbol);
    printf("Point group intl:  %s\n", spgtype.pointgroup_international);
    printf("Point group Schoe: %s\n", spgtype.pointgroup_schoenflies);
    printf("Arithmetic cc num. %d\n", spgtype.arithmetic_crystal_class_number);
    printf("Arithmetic cc sym. %s\n", spgtype.arithmetic_crystal_class_symbol);
}
