MODULES := modules/avl-tree modules/range-tree modules/triangle-mesh

all:
	@for mod in $(MODULES); do \
		echo "===> Compilando $$mod..."; \
		$(MAKE) -C $$mod || exit 1; \
	done
	@echo "Todos os módulos compilados com sucesso."

clean:
	@for mod in $(MODULES); do \
		echo "===> Limpando $$mod..."; \
		$(MAKE) -C $$mod clean || exit 1; \
	done
	@echo "Limpeza concluída."

test-avl:
	@$(MAKE) -C modules/avl-tree run

test-rt:
	@$(MAKE) -C modules/range-tree run

test-mesh:
	@$(MAKE) -C modules/triangle-mesh run mesh=meshes/tetra.obj

.PHONY: all clean test-avl test-rt test-mesh
