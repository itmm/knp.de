module Jekyll
  class RenderTimeTag < Liquid::Tag
    def initialize(tag_name, text, tokens)
      super
      parts = text.split(" ");
      @filename = parts[0];
      @type = parts[1..(-1)].join(" ") || "text";
      path = File.join('_site', @filename);
      if File.exists?(path)
        @content = File.read(path)
      else
        @content = ""
      end
      @highlighter = Liquid::Template.tags["highlight"].new(
          "highlight", @type, [@content, "{% endhighlight %}"]);
    end

    def render(context)
      link = "<a href=\"#{@filename}\">#{@filename}:</a>"
      code = @highlighter.render(context)
      "#{link}\n\n#{code}"
    end
  end
end

Liquid::Template.register_tag('inlineh', Jekyll::RenderTimeTag)
